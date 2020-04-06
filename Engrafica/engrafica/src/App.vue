<template>
  <v-app>

    <v-navigation-drawer app v-model="drawer" clipped class="primary" temporary >
        <div @mouseleave="hide($event)">
            <DocList/>
        </div>
    </v-navigation-drawer>

    <v-content app>
      <div @mouseover="move($event)" @mouseenter="enter($event)" >
        <DocView/>
      </div>
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
            this.drawer=false;
        },

        //
        move: function(event) {
            //console.log(event.clientX);
            if(event.clientX < 100) {
                this.show();
            }
        },

        //
        enter: function(event) {
            console.log(event.clientX);
            if(event.clientX < 100) {
                this.show();
            }
        },
    }

};
</script>
