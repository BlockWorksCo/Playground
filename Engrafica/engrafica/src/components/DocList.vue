<template>
     <v-list dense>

        <v-list-item>
          <v-list dense>
            <v-list-item>
              <v-title>Search</v-title>
            </v-list-item>
            <v-list-item>
              <v-text-field shaped outlined clearable dense filled rounded> Documents </v-text-field>
            </v-list-item>
          </v-list>
        </v-list-item>

        <DocListItem/>
        <DocListItem/>
        <DocListItem/>
        <DocListItem/>
        <DocListItem/>
        <DocListItem/>

      </v-list>
</template>

<script>
  import DocListItem from '../components/DocListItem';

  export default {
    name: 'DocList',

    components: {
      DocListItem,
    },

    data: () => ({
      ecosystem: [
        {
          text: 'vuetify-loader',
          href: 'https://github.com/vuetifyjs/vuetify-loader',
        },
        {
          text: 'github',
          href: 'https://github.com/vuetifyjs/vuetify',
        },
        {
          text: 'awesome-vuetify',
          href: 'https://github.com/vuetifyjs/awesome-vuetify',
        },
      ],
      importantLinks: [
        {
          text: 'Documentation',
          href: 'https://vuetifyjs.com',
        },
        {
          text: 'Chat',
          href: 'https://community.vuetifyjs.com',
        },
        {
          text: 'Made with Vuetify',
          href: 'https://madewithvuejs.com/vuetify',
        },
        {
          text: 'Twitter',
          href: 'https://twitter.com/vuetifyjs',
        },
        {
          text: 'Articles',
          href: 'https://medium.com/vuetify',
        },
      ],
      whatsNext: [
        {
          text: 'Explore components',
          href: 'https://vuetifyjs.com/components/api-explorer',
        },
        {
          text: 'Select a layout',
          href: 'https://vuetifyjs.com/layout/pre-defined',
        },
        {
          text: 'Frequently Asked Questions',
          href: 'https://vuetifyjs.com/getting-started/frequently-asked-questions',
        },
      ],
    }),
  }
</script>
