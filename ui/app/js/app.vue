<template>
  <span>
    <h1 class="title">Ingosi</h1>
    <h2 class="subtitle">Configuration du module pour Umtapo</h2>
    <progress-bar :current-step="currentStep" />
    <div class="content">
      <connection-step @done="goToNextStep" @loading="setLoading" @loaded="stopLoading" v-if="currentStep === 1" />
      <info-step @deviceInformation="receiveDeviceInformation" @done="goToNextStep" @loading="setLoading" @loaded="stopLoading" v-if="currentStep === 2" />
      <wifi-step @wifiConfig="receiveWifiConfig" @done="goToNextStep" @loading="setLoading" @loaded="stopLoading" v-if="currentStep === 3" />
      <mqtt-step @mqttConfig="receiveMqttConfig" @done="goToNextStep" v-if="currentStep === 4" />
      <sending-step :configuration="config" @loading="setLoading" @loaded="stopLoading" v-if="currentStep === 5" />

      <div v-if="loading" class="notification">
        <span class="button is-loading">Chargement</span> {{ loadingText }}
      </div>
    </div>
  </span>
</template>

<script>
import ProgressBar from './progressbar.vue'

import ConnectionStep from './steps/connection.vue'
import InfoStep from './steps/info.vue'
import WifiStep from './steps/wifi.vue'
import MqttStep from './steps/mqtt.vue'
import SendingStep from './steps/sending.vue'

export default {
  data () {
    return {
      loading: false,
      loadingText: '',
      currentStep: 1,
      deviceInformation: {},
      config: {
        "name": "Ingosi module",
        "ota": {
          "enabled": true
        }
      }
    }
  },
  methods: {
    goToNextStep: function () {
      this.currentStep++
    },
    setLoading: function (text) {
      this.loadingText = text
      this.loading = true
    },
    stopLoading: function () {
      this.loading = false
    },
    receiveDeviceInformation: function (deviceInformation) {
      this.deviceInformation = deviceInformation
    },
    receiveWifiConfig: function (config) {
      this.config.wifi = config
    },
    receiveMqttConfig: function (config) {
      this.config.mqtt = config
    }
  },
  components: {
    ProgressBar,
    ConnectionStep,
    InfoStep,
    WifiStep,
    MqttStep,
    SendingStep
  }
}
</script>

<style scoped>
  h1, h2 {
    text-align: center;
  }
</style>

<style>
  html {
    background: #fff;
  }

  input:active, input:focus {
    border-color: #076A92 !important;
  }

  .checkbox:hover input:checked, .checkbox input:checked {
    background: #076A92 !important;
    border-color: #27476F !important;
  }

  .button.is-primary {
    background: #076A92;
  }

  .button.is-primary:hover, .button.is-primary:focus {
    background: #27476F;
  }
</style>

<style scoped>
  .notification .button.is-loading {
    background: transparent;
    border: none;
  }
</style>
