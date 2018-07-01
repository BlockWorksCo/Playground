
<template>
  <v-container fluid>
    <v-slide-y-transition mode="out-in">
      <v-layout>

        <v-flex xs6>
           <v-card class="ma-5">
            <v-card-media src="https://raw.githubusercontent.com/ijklim/simon-game/gh-pages/assets/img/bg--game-pad.jpg" height="200px" ></v-card-media>
            <v-card-title primary-title>
              <div>
                <h3 class="headline mb-0">Cellular RSSI</h3>
                <div>Make continuous measurements of the RSSI on the cellular modem</div>
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
  name: 'Gateway',
  props: {
    msg: String
  }
}
</script>

