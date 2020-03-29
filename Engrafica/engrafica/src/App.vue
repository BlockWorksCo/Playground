<template>
  <v-app>

    <v-app-bar app clipped-left color="primary" dark flat class="deep-purple accent-4" >

      <div class="d-flex align-center">
        <v-img
          alt="Engrafica Logo"
          class="shrink mr-2"
          contain
          src="./assets/logo.png"
          transition="scale-transition"
          width="40"
        />

        <v-img
          alt="Vuetify Name"
          class="shrink mt-1 hidden-sm-and-down"
          contain
          min-width="100"
          src="./assets/EngraficaName.png"
          width="100"
        />
      </div>

      <v-spacer></v-spacer>

      <v-btn
        href="https://github.com/vuetifyjs/vuetify/releases/latest"
        target="_blank"
        text
      >
        <span class="mr-2">Latest Release</span>
        <v-icon>mdi-open-in-new</v-icon>
      </v-btn>
    </v-app-bar>

    <v-navigation-drawer app v-model="drawer" clipped class="deep-purple accent-3">
        <DocList/>
    </v-navigation-drawer>

    <v-content app>
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
};
</script>
