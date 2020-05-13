<template>
  <v-app>

    <v-navigation-drawer app v-model="drawer" clipped class="primary" temporary >
        <div @mouseleave="hide($event)">
            <DocList/>
        </div>
    </v-navigation-drawer>

    <v-content app>
      <div id="appSidebar" @mouseover="move($event)" @mouseenter="enter($event)" >
        <PDFView :filename="filename" :src="src" @mouseover="move($event)" @mouseenter="enter($event)" />
      </div>
    </v-content>

  </v-app>
</template>


<script>

import DocList from './components/DocList';
//import DocView from './components/DocView';
import PDFView from './views/PDFView';

export default {
  name: 'App',

  components: {
    //DocView,
    DocList,
    PDFView,
  },

  data: () => ({
    //
    drawer: true,

    //
    filename: "https://0.0.0.0:8081/MAX7400-MAX7407.pdf",

    //
    src: "https://0.0.0.0:8081/MAX7400-MAX7407.pdf",
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


<style>


    #appSidebar {
        height: 100%;
    }

</style>
