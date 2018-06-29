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
    >
      <v-list>
        <v-list-tile
          value="true"
          v-for="(item, i) in items"
          :key="i"
        >
          <v-list-tile-action>
            <v-icon v-html="item.icon"></v-icon>
          </v-list-tile-action>
          <v-list-tile-content>
            <router-link :to=item.target>{{ item.title }}</router-link>
          </v-list-tile-content>
        </v-list-tile>
      </v-list>
    </v-navigation-drawer>
    <v-toolbar
      app
      :clipped-left="clipped"
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
    <v-navigation-drawer
      temporary
      :right="right"
      v-model="rightDrawer"
      fixed
      app
    >
      <v-list>
        <v-list-tile @click="right = !right">
          <v-list-tile-action>
            <v-icon>compare_arrows</v-icon>
          </v-list-tile-action>
          <v-list-tile-title>Switch drawer (click me)</v-list-tile-title>
        </v-list-tile>
      </v-list>
    </v-navigation-drawer>
    <v-footer :fixed="fixed" app>
      <span>&copy; 2017</span>
    </v-footer>
  </v-app>
</template>

<script>
import HelloWorld from './components/HelloWorld'

export default {
  name: 'App',
  components: {
    HelloWorld
  },
  data () {
    return {
      chevronType:'chevron_left',
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
            target: "two",
        },
        {
            icon: 'router',
            title: 'GW200',
            target: "three",
        },
        {
            icon: 'business',
            title: 'Production',
            target: "four",
        },
        {
            icon: 'computer',
            title: 'Host Agent',
            target: "five",
        },
      ],
      miniVariant: false,
      right: true,
      rightDrawer: false,
      title: 'OmniBox'
    }
  }
}
</script>
