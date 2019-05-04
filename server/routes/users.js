const errors = require('restify-errors');
const User = require('../models/User');
const bcrypt = require("bcryptjs");
const auth = require("../auth");
const jwt = require("jsonwebtoken");
const config = require('../config');

const generateToken = user => {
    const token = jwt.sign(user.toJSON(), config.JWT_SECRET, {expiresIn: '15m'});
    const { iat, exp } = jwt.decode(token);
    return {iat, exp, token};
}

module.exports = server => {

    /**
     * Authenticates the user and dispatches a token
     */
    server.post('/auth', async (req, res, next) => {
        let username = null;
        let password = null;
        try {
            ({username, password} = req.body);
            const user = await auth.auth(username, password);

            res.send(generateToken(user));

            next();
        } catch (err) {
            console.log(`Unauthorized login access ${username}`);
            return next(new errors.UnauthorizedError());
        }
    });

    /** 
     * Light registration occurs when the user provides only a username. S/he is registered
     * but its identity is bound to the token dispatched by this method. This token can
     * be used to update the leaderboard and to register with the same name.
     */
    server.post('/lightRegistration', async (req, res, next) => {
        try {
            const {username} = req.body;

            if (!username || username.length < 4) 
                return next(new errors.BadRequestError('Username should contain at least four characters'));

            // Check whether the username is already in use
            const alreadyExists = await User.findOne({username}, {_id: 1});
            if (alreadyExists !== null) 
                return next(new errors.InvalidCredentialsError('The username you chose already exists, please try something different'));
            
            // Creates a new user
            const user = new User({
                username
            });
            user.save();

            res.send(generateToken(user));
            next();
         } catch (err) {
            console.log(err);
            return next(new errors.InternalError('Cannot process request'));
         }
    });

    /**
     * Registers a user. username and password must be provided if username
     * is already in use and it does not match the one provided with the token
     * the request is rejected.
     */
    server.post('/registration', async (req, res, next) => {
        try {
            const {username, password} = req.body;
            let sessionUsername = null;
            if (req.headers && req.headers.authorization) {
                // removes the jwt prefix
                const decoded = jwt.decode(req.headers.authorization.substr(4));
                sessionUsername = decoded.username
            }

            // Check whether the username is already in use
            // if it is already in use but it matches the one provided with the token then it is ok
            const alreadyExists = await User.findOne({username});
            if (alreadyExists !== null && alreadyExists.isRegistered)
                return next(new errors.InvalidCredentialsError('This user is already registered'));
            if (alreadyExists !== null && username !== sessionUsername)
                return next(new errors.InvalidCredentialsError('The username you chose already exists, please try something different'));
            if (!username || username.length < 4) 
                return next(new errors.InvalidCredentialsError('Username should contain at least four characters'));
            if (!password || password.length < 4)
                return next(new errors.InvalidCredentialsError('Password should contain at least four characters'));

            const user = alreadyExists || new User({
                username,
                password
            });
            user.isRegistered = true;

            // encrypt password and save
            bcrypt.genSalt(10, (err, salt) => {
                bcrypt.hash(password, salt, async (err, hash) => {
                    user.password = hash;
                    try {
                        await user.save();
                        res.send(generateToken(user));
                        next();
                    } catch(err) {
                        return next(new errors.InternalError(err.message));
                    }
                });
            });
        } catch (err) {
            console.log(err);
            return next(new errors.InternalError('Cannot process request'));
        }
    });

    /**
     * This method is used to update the leaderboard for a certain
     * user. The user must provide a token proving s/he is authenticated
     */
    server.put('/updateScore', async (req, res, next) => {
        try {
            const submittedScore = req.body.score;
            await User.findOneAndUpdate({username: req.user.username,
                                             bestScore: { $lt: submittedScore }}, {bestScore: submittedScore});
            res.send(200);
            next();
        } catch (err) {
            console.log(err);
            return next(new errors.InternalError('Cannot process request'));
        }
    });

    /**
     * Fetches the entire leaderboard ordered by score
     */
    server.get('/leaderboard', async (req, res, next) => {
        try {
            let current = null;
            let rank = 1;
            const leaderboard = await User.find({}, {username: 1, bestScore: 1, updatedAt: 1})
                .sort({bestScore: -1});

            const leaderboardWithRank = leaderboard.map(doc => {
                const data = doc.toObject();
                if (current === null) 
                    current = doc.bestScore;
                if (doc.bestScore !== current) {
                    current = doc.bestScore;
                    rank += 1;
                }
                data.rank = rank;
                return data;
            });

            res.send(leaderboardWithRank);
            next();
        } catch (err) {
            console.log(err);
            return next(new errors.InternalError('Cannot process request'));
        }
    });
}