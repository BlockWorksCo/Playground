import '@babel/polyfill'
import Vue from 'vue'
import './plugins/vuetify'
import App from './App.vue'
import router from './router'

Vue.config.productionTip = false

new Vue({
  router,
  render: h => h(App)
}).$mount('#app')

/*

Home
====

    RF
    ==
    Radio test to slave
    get rssi
    set tx power

    IPA
    ===
    nuke
    program bootloader
    program firmware
    Set config parameter

    GW
    ==
    Program bootloader
    program firmware

    Production
    ==========
    Move out of production mode
    Program credentials

    Host Agent
    ==========
    Run host agent
    Restart host agent
    SMA
    NMS

About
=====



*/
