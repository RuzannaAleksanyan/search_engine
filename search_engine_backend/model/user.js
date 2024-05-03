const mongoose = require('mongoose')

const UserSchema = new mongoose.Schema(
	{
		username: { type: String, required: true, unique: true },
		email: { type: String, required: true, unique: true },
		password: { type: String, required: true },
		flag: { type: String },
		image: { type: String },
		shortcut: [
			{
				url: { type: String },
				name: { type: String }
			}
		],
		history: [{ type: String }]
	},
	{ collection: 'users' }
)

const model = mongoose.model('UserSchema', UserSchema)

module.exports = model
