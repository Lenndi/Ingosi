<template>
  <span>
    <p>
      Entrez les informations MQTT. Vous trouverez toutes les informations nécessaires dans l'espace Administration d'Umtapo.
    </p>

    <form @submit.prevent="sendDone">
      <label class="label" for="mqtt_broker_address">Adresse MQTT</label>
      <p class="control">
        <input v-model.trim="host" class="input" type="text" id="mqtt_broker_address" placeholder="Adresse MQTT" pattern="^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$|^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\-]*[a-zA-Z0-9])\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\-]*[A-Za-z0-9])$" required />
        <span class="help">Requis.</span>
      </p>

      <label class="label" for="mqtt_broker_port">Port MQTT</label>
      <p class="control">
        <input v-model.number="port" class="input" type="number" step="1" min="1" max="65535" id="mqtt_broker_port" placeholder="Port MQTT" required />
        <span class="help">Requis.</span>
      </p>

      <p class="control">
        <label class="checkbox">
          <input v-model="auth" type="checkbox" /> Utiliser l'authentification MQTT
        </label>
      </p>

      <div v-if="auth">
        <label class="label" for="mqtt_username">Nom d'utilisateur MQTT</label>
        <p class="control">
          <input v-model.trim="username" class="input" type="text" id="mqtt_username" placeholder="Nom d'utilisateur" required />
          <span class="help">Requis.</span>
        </p>

        <label class="label" for="mqtt_password">Mot de passe MQTT</label>
        <p class="control is-grouped">
          <!-- v-model does not support dynamic :type -->
          <input v-if="passwordClearText" type="text" class="input" v-model.trim="password" id="mqtt_password" placeholder="Mot de passe" />
          <input v-else type="password" class="input" v-model.trim="password" id="mqtt_password" placeholder="Mot de passe" />
          <label class="checkbox">
            <input type="checkbox" v-model="passwordClearText" /> Afficher le mot de passe
          </label>
        </p>
        <span class="help">Requis.</span>

        <br/>
      </div>

      <p class="control">
        <button type="submit" :disabled="!formIsValid" class="button is-primary">Suivant</button>
      </p>
    </form>
  </span>
</template>

<script>

export default {
  data () {
    return {
      passwordClearText: false,
      host: null,
      port: 1883,
      baseTopic: null,
      auth: false,
      username: null,
      password: null
    }
  },
  computed: {
    formIsValid: function () {
      if (!this.host) return false
      if (!this.port) return false

      if (this.auth) {
        if (!this.username) return false
        if (!this.password) return false
      }

      return true
    }
  },
  methods: {
    sendDone: function () {
      const mqtt = {}
      mqtt.host = this.host
      mqtt.port = this.port
      if (this.baseTopic) mqtt['base_topic'] = this.baseTopic

      mqtt.auth = false
      if (this.auth) {
        mqtt.auth = true
        mqtt.username = this.username
        mqtt.password = this.password
      }

      this.$emit('mqttConfig', mqtt)
      this.$emit('done')
    }
  }
}
</script>
