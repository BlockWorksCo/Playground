
<template>
  <v-container fluid>
    <v-slide-y-transition mode="out-in">
      <v-layout wrap=true>

        <v-flex xs6>
           <v-card class="ma-5">
            <v-card-media src="https://raw.githubusercontent.com/ijklim/simon-game/gh-pages/assets/img/bg--game-pad.jpg" height="200px" ></v-card-media>
            <v-card-title primary-title>
              <div>
                <h3 class="headline mb-0">Take node out of production mode</h3>
                <div>Take node out of production mode</div>
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
                <h3 class="headline mb-0">Configure credentials</h3>
                <div>Program the certificates and keys required for normal operation on an OmniMesh network</div>
              </div>
            </v-card-title>
            <v-card-actions>
              <v-btn to="ConfigureCredentials" color=success>Do it...</v-btn>
            </v-card-actions>
          </v-card>
        </v-flex>

        <v-flex xs6>
           <v-card class="ma-5">
            <v-card-media src="https://raw.githubusercontent.com/ijklim/simon-game/gh-pages/assets/img/bg--game-pad.jpg" height="200px" ></v-card-media>
            <v-card-title primary-title>
              <div>
                <h3 class="headline mb-0">Nuke</h3>
                <div>Remove all software, certificates and keys required for normal operation on an OmniMesh network</div>
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

