module.exports = {
  "transpileDependencies": [
    "vuetify"
  ],
    configureWebpack: {
        module: {
          rules: [{
            test: /\.(png|gif|cur|jpg)$/, loader: 'file-loader', query: { limit: 8192 }
          }]
        }

    },
}
