
<template>
  <v-container fluid>
    <v-slide-y-transition mode="out-in">
      <v-layout wrap=true>

        <v-flex xs6>
           <v-card class="ma-5">
            <v-card-media v-bind:src="require('@/assets/SignalStrength.png')" contain height=200px />
            <v-card-title primary-title>
              <div>
                <h3 class="headline mb-0">Measure RSSI</h3>
                <div>Make continuous measurements of the RSSI on the node</div>
              </div>
            </v-card-title>
            <v-card-actions>
              <v-btn to="MeasureRSSI" color=success>Do it...</v-btn>
            </v-card-actions>
          </v-card>
        </v-flex>

        <v-flex xs6 sm6>
           <v-card class="ma-5">
            <v-card-media v-bind:src="require('@/assets/Transmitter.png')" contain height=200px />
            <v-card-title primary-title>
              <div>
                <h3 class="headline mb-0">Continuous Transmit</h3>
                <div>Put the node into continuous transmit mode</div>
              </div>
            </v-card-title>
            <v-card-actions>
              <v-btn color=success>Do it...</v-btn>
            </v-card-actions>
          </v-card>
        </v-flex>

        <v-flex xs6 sm6>
           <v-card class="ma-5">
            <v-card-media src="https://raw.githubusercontent.com/ijklim/simon-game/gh-pages/assets/img/bg--game-pad.jpg" height="200px" ></v-card-media>
            <v-card-title primary-title>
              <div>
                <h3 class="headline mb-0">Configure RF parameters</h3>
                <div>Set the RF parameters such as Transmit power, thresholds, etc</div>
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
  name: 'Radio',
  props: {
    msg: String
  }
}
</script>

