<template>
  <p>
    Bienvenue sur la page de configuration de votre module Ingosi. Avec Ingosi, vous allez pouvoir gérer les entrées et sorties documentaires d'un simple geste.
  </p>
</template>

<script>
import axios from 'axios'
import {API_URL, HEARTBEAT_ATTEMPT_INTERVAL} from '../constants'

export default {
  data () {
    return {
      requestOnGoing: false,
      interval: null
    }
  },
  mounted () {
    this.$emit('loading', 'Chargement des données du module...')
    this.interval = setInterval(this.sendRequest, HEARTBEAT_ATTEMPT_INTERVAL)
  },
  methods: {
    sendRequest: function () {
      if (this.requestOnGoing) return

      this.requestOnGoing = true
      axios.get(`${API_URL}/heart`).then((res) => {
        clearInterval(this.interval)
        this.$emit('loaded')
        this.$emit('done')
      }).catch((err) => {
        this.requestOnGoing = false
      })
    }
  }
}
</script>
