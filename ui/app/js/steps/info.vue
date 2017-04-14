<template>
  <span>
    <span v-if="!loading">
      <p>
        Voici les informations de votre module :
      </p>

      <ul>
        <li><span class="icon is-small"><i class="fa fa-hashtag"></i></span> <b>ID du module:</b> <span class="tag is-dark">{{ apiResponse.hardware_device_id }}</span></li>
        <li>
          <span class="icon is-small">
            <i class="fa fa-font"></i>
          </span>
          <b>Firmware</b>
          <ul>
            <li><b>Nom:</b> <span class="tag is-dark">{{ apiResponse.firmware.name }}</span></li>
            <li><b>Version:</b> <span class="tag is-dark">{{ apiResponse.firmware.version }}</span></li>
          </ul>
        </li>
      </ul>

      <a class="button is-primary" @click="sendDone">Suivant</a>
    </span>
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
  mounted () {
    this.$emit('loading', 'Récupération des informations du module...')
    axios.get(`${API_URL}/device-info`).then((res) => {
      this.$emit('loaded')
      this.apiResponse = res.data
      this.$emit('deviceInformation', this.apiResponse)
      this.loading = false
    }).catch((err) => {
    })
  },
  methods: {
    sendDone: function () {
      this.$emit('done')
    }
  }
}
</script>


<style scoped>
  b {
    font-weight: bold;
  }
</style>
