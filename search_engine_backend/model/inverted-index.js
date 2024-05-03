const mongoose = require('mongoose')

const InvertedSchema = new mongoose.Schema(
	{
		word: { type: String},
		urls: [
			{
				type: Object
			}
		],
	},
	{ collection: 'inverted_index_en' }
)

const model = mongoose.model('InvertedSchema', InvertedSchema)

module.exports = model
