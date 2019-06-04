const bcrypt = require('bcryptjs');
const mongoose = require('mongoose');
const User = mongoose.model('User');
const crypto = require('crypto')
const config = require('./config');

exports.auth = (username, password) => {
    return new Promise(async (resolve, reject) => {
        try {
            const user = await User.findOne({username});

            bcrypt.compare(password, user.password, (err, isMatch) => {
                if (err) throw err;
                if (isMatch) {
                    resolve(user);
                } else {
                    reject('Authentication failed');
                }
            })

        } catch (err) {
            // not found
            reject('Authentication faild');
        }
    });
}

exports.checkHmac = (score, hmac) => {
    let hash = crypto.createHmac('SHA256', config.SCORE_SECRET).update(score).digest('base64');
    hmac = hmac.trim();
    hash = hash.trim();
    return hash === hmac;
}