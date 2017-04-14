<template>
  <span v-if="!loading">
    <div v-if="apiResponse.success === true" class="notification is-success">
      La configuration est effectuée. Le module va redémarrer. Vous pouvez fermer cette page.
    </div>
    <div v-else class="notification is-danger">
      Une erreur s'est produite pendant l'enregistrement de la configuration : {{ apiResponse.error }}
    </div>
  </span>
</template>

<script>
import axios from 'axios'
import {API_URL} from '../constants'

export default {
  data () {
    return {
      loading: true,
      apiResponse: null
    }
  },
  props: ['configuration'],
  mounted () {
    this.$emit('loading', 'Sending configuration to the device...')
    axios.put(`${API_URL}/config`, this.configuration).then((res) => {
      this.$emit('loaded')
      this.apiResponse = res.data
      this.loading = false
    }).catch((err) => {
      this.$emit('loaded')
      if (err.response) this.apiResponse = err.response.data
      else this.apiResponse = { error: err.message }
      this.loading = false
    })
  },
}
</script>
