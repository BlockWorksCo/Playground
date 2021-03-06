import Vue from 'vue'
import Router from 'vue-router'
import About from './views/About.vue'
import HelloWorld from './components/HelloWorld.vue'
import Radio from './components/Radio.vue'
import Gateway from './components/Gateway.vue'
import Node from './components/Node.vue'
import Production from './components/Production.vue'
import HostAgent from './components/HostAgent.vue'
import ConfigureCredentials from './components/ConfigureCredentials.vue'
import MeasureRSSI from './components/MeasureRSSI.vue'
import DebugOutput from './components/DebugOutput.vue'

Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '/',
      name: 'home',
      component: HelloWorld
    },
    {
      path: '/about',
      name: 'about',
      component: About
    },
    {
      path: '/radio',
      name: 'radio',
      component: Radio
    },
    {
      path: '/gateway',
      name: 'Gateway',
      component: Gateway
    },
    {
      path: '/node',
      name: 'Node',
      component: Node
    },
    {
      path: '/production',
      name: 'Production',
      component: Production
    },
    {
      path: '/hostagent',
      name: 'HostAgent',
      component: HostAgent
    },
    {
      path: '/configurecredentials',
      name: 'ConfigureCredentials',
      component: ConfigureCredentials
    },
    {
      path: '/measurerssi',
      name: 'MeasureRSSI',
      component: MeasureRSSI
    },
    {
      path: '/debugoutput',
      name: 'DebugOutput',
      component: DebugOutput
    },
  ]
})
