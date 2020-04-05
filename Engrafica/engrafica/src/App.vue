<template>
  <v-app>

    <v-navigation-drawer app v-model="drawer" clipped class="primary" temporary>
        <DocList/>
    </v-navigation-drawer>

    <v-content app>
        <v-btn z-index=999 color="rgb(255, 0, 0, 0.0)" flat fixed dark @mouseover="show()" @mouseleave="hide()" @click.stop="drawer = !drawer" >toggle</v-btn>
      <DocView/>
    </v-content>

  </v-app>
</template>


<script>

import DocList from './components/DocList';
import DocView from './components/DocView';

export default {
  name: 'App',

  components: {
    DocView,
    DocList,
  },

  data: () => ({
    //
    drawer: true,

  }),

    //
    methods: {

        //
        show: function() {
            this.drawer=true;
        },

        //
        hide: function() {
            this.drawer=true;
        }
    }

};
</script>
