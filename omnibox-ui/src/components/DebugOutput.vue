
<template>
  <v-container fluid>
    <v-slide-y-transition mode="out-in">
      <v-layout wrap=true>

        <v-flex xs12>
           <v-card class="ma-5">
            <v-card-title primary-title>
              <div>
                <h3 class="headline mb-0">Run host-agent</h3>

                    <b-form-textarea id="textarea1"
                      v-model="text"
                      placeholder="Enter something"
                      :rows="3"
                      :max-rows="6">
                    </b-form-textarea>
                    <pre class="mt-3">{{ text }}</pre>

              </div>
            </v-card-title>
            <v-card-actions>
              <v-btn color=success>Do it...</v-btn>
            </v-card-actions>
          </v-card>
        </v-flex>

      </v-layout>
    </v-slide-y-transition>
  </v-container>
</template>

<script>

export default {
  components: {},
  name: 'host-agent',
  props: {
    msg: String
  },
  data() {
     return {
        text : 'This is the text in the textarea box\n\nblaa\none\ntwo\nthree\nfour\nfive\nsix\nseven'
     }
  },
  created() {
    this.$options.interval = setInterval(this.periodicUpdate, 100);
  },
  methods: {
    periodicUpdate() {

       this.text = this.text + '['+Math.random()+']\n'; 

       var lineCount    = 0; 
       var i = 0;
       for(i=this.text.length; i>0; i--) {
         if(this.text[i] == '\n') {
            lineCount++;
            if(lineCount == 20) {
                this.text = this.text.slice( i,this.text.length );
                break;
            }
         }
       }

    }
  }
}
</script>

