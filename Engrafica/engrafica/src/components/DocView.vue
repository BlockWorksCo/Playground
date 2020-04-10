<template>
  <v-container fluid>

    <div id="buttons" class="ui grey three item inverted bottom fixed menu transition hidden">
      <a class="item" @click="page > 1 ? page-- : 1">
        <i class="left chevron icon"></i>
        Back
      </a>
      <a class="ui active item">
        {{page}} / {{ numPages ? numPages : '∞' }}
      </a>
      <a class="item" @click="page < numPages ? page++ : 1">
        Forward
        <i class="right chevron icon"></i>
      </a>
    </div>

    <div id="buttons" class="ui grey three item inverted bottom fixed menu transition hidden">
      <a class="item" @click="scale -= scale > 0.2 ? 0.1 : 0">
        <i class="left chevron icon" />
          Zoom -
      </a>
      <a class="ui active item">
        {{ formattedZoom }} %
      </a>
      <a class="item" @click="scale += scale < 2 ? 0.1 : 0">
        Zoom +
        <i class="right chevron icon" />
      </a>
    </div>

    <pdf :src="pdfdata" @mouseenter="move($event)" v-for="i in numPages" :key="i" :id="i" :page="i" :scale.sync="scale" style="width:100%;margin:20px auto;">
      <template slot="loading">
        loading content here...
      </template>
    </pdf>

    <v-speed-dial fab top right open-on-hover fixed direction="left" >
      <template v-slot:activator>
        <v-btn v-model="fab" color="blue darken-2" dark fab >
          <v-icon v-if="fab">mdi-close</v-icon>
          <v-icon v-else>mdi-account-circle</v-icon>
        </v-btn>
      </template>
      <v-btn fab dark small color="green" >
        <v-icon>mdi-pencil</v-icon>
      </v-btn>
      <v-btn fab dark small color="indigo" >
        <v-icon>mdi-plus</v-icon>
      </v-btn>
      <v-btn fab dark small color="red" >
        <v-icon>mdi-delete</v-icon>
      </v-btn>
    </v-speed-dial>


  </v-container>
</template>

<script>

  // https://github.com/arkokoley/pdfvuer
 
  import pdfvuer from 'pdfvuer'
  //import $ from 'jquery'

  export default {

    //
    name: 'DocView',

    //
    components: {
		pdf: pdfvuer
	},

    //
    data: () => ({
      page: 1,
      numPages: 0,
      scale: 'page-width',

      direction: 'top',
      fab: false,
      fling: false,
      hover: false,
      tabs: null,
      top: false,
      right: true,
      bottom: true,
      left: false,
      transition: 'slide-y-reverse-transition',
    }),

    //
    computed: {

      //
      formattedZoom () {
        return Number.parseInt(this.scale * 100);
      },

      //
      activeFab () {
        switch (this.tabs) {
          case 'one': return { class: 'purple', icon: 'account_circle' }
          case 'two': return { class: 'red', icon: 'edit' }
          case 'three': return { class: 'green', icon: 'keyboard_arrow_up' }
          default: return {}
        }
      },

    },

    //
    mounted () {
      this.getPdf()
    },

    //
    watch: {
      show: function (s) {
        if(s) {
          this.getPdf();
        }
      },

     //
     page: function (p) {
       if( window.pageYOffset <= this.findPos(document.getElementById(p)) || ( document.getElementById(p+1) && window.pageYOffset >= this.findPos(document.getElementById(p+1)) )) {
         // window.scrollTo(0,this.findPos(document.getElementById(p)));
         document.getElementById(p).scrollIntoView();
       }
     }

   },


     //
     methods: {

       getPdf () {
         var self = this;
          self.pdfdata = pdfvuer.createLoadingTask('https://0.0.0.0:8081/MAX7400-MAX7407.pdf');
          self.pdfdata.then(pdf => {
            self.numPages = pdf.numPages;
            window.onscroll = function() { 
              //changePage() 
              //stickyNav()  
            }

/*
            // Get the offset position of the navbar
            var sticky = $('#buttons')[0].offsetTop

            // Add the sticky class to the self.$refs.nav when you reach its scroll position. Remove "sticky" when you leave the scroll position
            function stickyNav() {
              if (window.pageYOffset >= sticky) {
                $('#buttons')[0].classList.remove("hidden")
              } else {
                $('#buttons')[0].classList.add("hidden")
              }
            }

            function changePage () {
              var i = 1, count = Number(pdf.numPages);
              do {
                if(window.pageYOffset >= self.findPos(document.getElementById(i)) && 
                    window.pageYOffset <= self.findPos(document.getElementById(i+1))) {
                  self.page = i
                }
                i++
              } while ( i < count)
              if (window.pageYOffset >= self.findPos(document.getElementById(i))) {
                self.page = i
              }
            }
*/

          });
        },

        //
        findPos(obj) {
          return obj.offsetTop;
        },
       }


  }
</script>



