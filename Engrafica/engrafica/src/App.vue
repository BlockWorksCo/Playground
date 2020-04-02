<template>
  <v-app>

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
