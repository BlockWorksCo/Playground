
import Vue from 'vue'

import {
  Vuetify,
  VApp,
  VNavigationDrawer,
  VFooter,
  VList,
  VBtn,
  VIcon,
  VGrid,
  VToolbar,
  transitions,
  VStepper,
  VCard,
} from 'vuetify'
import 'vuetify/src/stylus/app.styl'

Vue.use(Vuetify, {

  theme: {
      primary: "#FF9800",
      secondary: "#FFB74D",
      accent: "#FF9800",
      error: "#f44336",
      warning: "#ffeb3b",
      info: "#2196f3",
      success: "#4caf50"
  },

  components: {
    VApp,
    VNavigationDrawer,
    VFooter,
    VList,
    VBtn,
    VIcon,
    VGrid,
    VToolbar,
    transitions,
    VStepper,
    VCard,
  },

})
