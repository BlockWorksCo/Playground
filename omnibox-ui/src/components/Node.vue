


<template>
  <v-container fluid>
    <v-slide-y-transition mode="out-in">
      <v-layout>
        <v-flex xs12>
          <v-stepper v-model="e1">
            <v-stepper-header>

              <v-stepper-step :complete="e1 > 1" step="1">Connect Node to programmer</v-stepper-step>
              <v-divider></v-divider>
              <v-stepper-step :complete="e1 > 2" step="2">Choose bootloader</v-stepper-step>
              <v-divider></v-divider>
              <v-stepper-step :complete="e1 > 3" step="3">Choose firmware</v-stepper-step>
              <v-divider></v-divider>
              <v-stepper-step :complete="e1 > 4" step="4">Program the images</v-stepper-step>
              <v-divider></v-divider>
              <v-stepper-step :complete="e1 > 5" step="5">Reboot and check</v-stepper-step>

            </v-stepper-header>

            <v-stepper-items>
              <v-stepper-content step="1">
                <v-card class="mb-5" color="grey lighten-1" height="200px" >
                    <v-card-title primary-title>
                      <div>
                        <h3 class="headline mb-0">Step 1: Physical connections</h3>
                        <div>Make sure this is plugged into that, and that into this.</div>
                      </div>
                    </v-card-title>
                </v-card>

                <v-btn color="primary" @click="e1 = 2" >
                  Next
                </v-btn>

                <v-btn flat>Cancel</v-btn>
              </v-stepper-content>

              <v-stepper-content step="2">

                <v-card class="mb-5" color="grey lighten-1" height="200px" >
                    <v-card-title primary-title>
                      <div>
                        <h3 class="headline mb-0">Step 2: Choose a bootloader image</h3>
                        <div>This should be called bootloader.elf or something similar.</div>
                      </div>
                    </v-card-title>
                </v-card>

                <v-layout>
                    <v-flex xs2>
                        <upload-button title="Choose Bootloader..." :fileChangedCallback="onFileSelected" /> 
                    </v-flex>
                    <v-flex xs1>
                        <v-btn flat @click="e1=1">Back</v-btn>
                    </v-flex>
                </v-layout>

              </v-stepper-content>

              <v-stepper-content step="3">

                <v-card class="mb-5" color="grey lighten-1" height="200px" >
                    <v-card-title primary-title>
                      <div>
                        <h3 class="headline mb-0">Step 3: Choose a firmware image</h3>
                        <div>This should be called agent.elf or something similar.</div>
                      </div>
                    </v-card-title>
                </v-card>

                <v-layout>
                    <v-flex xs2>
                        <upload-button title="Choose Firmware..." :fileChangedCallback="onFileSelected" /> 
                    </v-flex>
                    <v-flex xs1>
                        <v-btn flat @click="e1=2">Back</v-btn>
                    </v-flex>
                </v-layout>

              </v-stepper-content>

              <v-stepper-content step="4">

                <v-card class="mb-5" color="grey lighten-1" height="200px" >
                    <v-card-title primary-title>
                      <div>
                        <h3 class="headline mb-0">Step 4: Program the board</h3>
                        <div>Bootloader and firmware will now be programmed onto the node</div>
                      </div>
                    </v-card-title>
                </v-card>

                <v-btn color="primary" @click="e1 = 5" >
                  Continue
                </v-btn>

                <v-btn flat @click="e1=3">Back</v-btn>
              </v-stepper-content>

              <v-stepper-content step="5">

                <v-card class="mb-5" color="grey lighten-1" height="200px" >
                    <v-card-title primary-title>
                      <div>
                        <h3 class="headline mb-0">Step 5: Check</h3>
                        <div>Check its all working.</div>
                      </div>
                    </v-card-title>
                </v-card>

                <v-btn color="primary" @click="e1 = 1" >
                  Program another node
                </v-btn>

              </v-stepper-content>


            </v-stepper-items>
          </v-stepper>
        </v-flex>
      </v-layout>
    </v-slide-y-transition>
  </v-container>
</template>

<script>

import axios from 'axios';
import UploadButton from 'vuetify-upload-button';

export default {
  name: 'Node',
  components: {
    'upload-button': UploadButton,
  },
  props: {
    msg: String
  },
  data () {
      return {
        e1: 0,
        selectedFile: null,
      }
    },
  methods: {
    onFileSelected(file) {

        this.selectedFile = file;

        const fd = new FormData();
        const names = ['bootloader','firmware'];
        fd.append( names[this.e1-2], this.selectedFile, this.selectedFile.name );
        axios.post('http://localhost:5001/', fd).then( res => {alert('Done:' + res)} );

        this.e1 = this.e1+1;
    },
  },
}
</script>

