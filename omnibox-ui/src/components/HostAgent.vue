
<template>
  <v-container fluid>
    <v-slide-y-transition mode="out-in">
      <v-layout wrap=true>

        <v-flex xs6>
           <v-card class="ma-5">
            <v-card-media src="https://raw.githubusercontent.com/ijklim/simon-game/gh-pages/assets/img/bg--game-pad.jpg" height="200px" ></v-card-media>
            <v-card-title primary-title>
              <div>
                <h3 class="headline mb-0">Run host-agent</h3>
                <div>Startup a host-agent on the OmniBox, talking to a meter using an optical probe</div>
              </div>
            </v-card-title>
            <v-card-actions>
              <v-btn color=success>Do it...</v-btn>
            </v-card-actions>
          </v-card>
        </v-flex>

        <v-flex xs6>
           <v-card class="ma-5">
            <v-card-media src="https://raw.githubusercontent.com/ijklim/simon-game/gh-pages/assets/img/bg--game-pad.jpg" height="200px" ></v-card-media>
            <v-card-title primary-title>
              <div>
                <h3 class="headline mb-0">Watch debug output</h3>
                <div>See the debug output from an attached node.</div>
              </div>
            </v-card-title>
            <v-card-actions>
              <v-btn color=success>Do it...</v-btn>
            </v-card-actions>
          </v-card>
        </v-flex>

        <v-flex xs6>
           <v-card class="ma-5">
            <v-card-media src="https://raw.githubusercontent.com/ijklim/simon-game/gh-pages/assets/img/bg--game-pad.jpg" height="200px" ></v-card-media>
            <v-card-title primary-title>
              <div>
                <h3 class="headline mb-0">Send CoAP requests to the node</h3>
                <div>Send CoAP requests to the node</div>
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
  }
}
</script>

