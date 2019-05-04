const mongoose = require('mongoose');
const timestamp = require('mongoose-timestamp');

const UserSchema = new mongoose.Schema({
    username: {
        type: String,
        required: true,
        trim: true
    },
    password: {
        type: String,
        required: false,
        default: null
    },
    bestScore: {
        type: Number,
        default: 0
    },
    isRegistered: {
        type: Boolean,
        default: false
    }
});

UserSchema.plugin(timestamp);

const User = mongoose.model('User', UserSchema);

module.exports = User;