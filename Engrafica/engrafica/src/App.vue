<template>
  <v-app>

    <v-app-bar app clipped-left hide-on-scroll color="primary" dark flat class="primary" >


      <div class="d-flex align-center">
        <v-app-bar-nav-icon @click="drawer=!drawer"></v-app-bar-nav-icon>
        <v-toolbar-title>Engrafica</v-toolbar-title>
      </div>

      <v-spacer></v-spacer>

        <span class="mr-2">Latest Release</span>
        <v-icon>mdi-open-in-new</v-icon>
    </v-app-bar>

    <v-navigation-drawer app v-model="drawer" clipped class="primary">
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
