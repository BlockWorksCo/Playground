<template>
  <v-app>
    <v-navigation-drawer
      persistent
      :mini-variant="miniVariant"
      :clipped="clipped"
      v-model="drawer"
      enable-resize-watcher
      hide-overlay="true"
      fixed
      app
      class="secondary"
    >
      <v-list class="secondary" >
        <v-list-tile
          class="white--text" active-class="white--text"
          value="true"
          v-for="item in items"
          :key=item.title
          :to=item.target
        >
          <v-list-tile-action>
            <v-icon v-html="item.icon"></v-icon>
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title>{{ item.title }}</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
      </v-list>
    </v-navigation-drawer>

    <v-toolbar
      app
      :clipped-left="clipped"
      color="primary"
    >

      <v-btn v-on:click="miniVariant=!miniVariant" :flat="true" :small="true" :round="true">
        <v-icon v-if="miniVariant==false">chevron_left</v-icon>
        <v-icon v-if="miniVariant==true">chevron_right</v-icon>
      </v-btn>

      <v-spacer></v-spacer>
      <v-toolbar-title v-text="title"></v-toolbar-title>
      <v-spacer></v-spacer>
    </v-toolbar>

    <v-content>
      <router-view />
    </v-content>

    <v-footer :fixed="fixed" app>
      <span>&copy; 2017</span>
    </v-footer>
  </v-app>
</template>

<script>

export default {
  name: 'App',
  components: {
  },
  data () {
    return {
      clipped: true,
      drawer: true,
      fixed: false,
      items: 
      [
        {
            icon: 'cell_wifi',
            title: 'Radio',
            target: "radio",
        },
        {
            icon: 'speaker_phone',
            title: 'IPA 1/3',
            target: "node",
        },
        {
            icon: 'router',
            title: 'GW200',
            target: "gateway",
        },
        {
            icon: 'business',
            title: 'Production',
            target: "production",
        },
        {
            icon: 'computer',
            title: 'Host Agent',
            target: "hostagent",
        },
      ],
      miniVariant: true,
      right: true,
      rightDrawer: false,
      title: 'OmniBox'
    }
  }
}
</script>
