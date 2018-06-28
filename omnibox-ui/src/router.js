import Vue from 'vue'
import Router from 'vue-router'
import About from './views/About.vue'
import HelloWorld from './components/HelloWorld.vue'

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
    }
  ]
})
