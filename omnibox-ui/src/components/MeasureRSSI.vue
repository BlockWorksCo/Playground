


<template>
  <v-container fluid>
    <v-slide-y-transition mode="out-in">
      <v-layout wrap=true>
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
                <v-card class="mb-5" color="grey lighten-1" height="600px" >
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

                <v-card class="mb-5" height="600px" >
                    <v-container wrap=true fluid>
                        <v-layout wrap=true>
                           <v-flex xs12 align-end flexbox>
                             <v-card-title primary-title>
                               <h3 class="headline mb-0">Step 2: Monitor RSSI values</h3>
                             </v-card-title>
                           </v-flex>
                           <v-flex xs12 align-end flexbox height=500px>
                                <div><line-chart :data="datacollection" :options="options" :height="500" :width="1000"/></div>
                           </v-flex>
                        </v-layout>
                    </v-container>

                      <div>
                        <div>This should be called bootloader.elf or something similar.</div>
                      </div>
                </v-card>

                <v-btn color="primary" @click="e1 = 3" >
                  Choose Bootloader
                </v-btn>

                <v-btn flat @click="e1=1">Back</v-btn>
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

                <v-btn color="primary" @click="e1 = 4" >
                  Choose firmware...
                </v-btn>

                <v-btn flat @click="e1=2">Back</v-btn>
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

  import LineChart from '@/components/LineChart.vue'

export default {
  name: 'MeasureRSSI',
  props: {
    msg: String
  },
  components: {LineChart},
  data () {
      return {
        e1: 0,

        datacollection: {
            //Data to be represented on x-axis
            labels: ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'],
            datasets: [{
               label: 'Data One',
               backgroundColor: '#f87979',
               pointBackgroundColor: 'white',
               borderWidth: 1,
               pointBorderColor: '#249EBF',
               //Data to be represented on y-axis
               data: [40, 80, 30, 50, 90, 10, 20, 40, 50, 70, 90, 100]
            }]
         },

        options: {
            scales: {
               yAxes: [{
                  ticks: {
                     beginAtZero: true
                  },
                  gridLines: {
                     display: true
                  }
               }],
               xAxes: [{
                  gridLines: {
                     display: true
                  }
               }]
            },
            legend: {
               display: false
            },
            responsive: false,
            maintainAspectRatio: false
         },

        myStyles: "max-width:100px max-height:300px width:333px height:444px",
      }

    },
}
</script>

