

<template>
 <textarea v-model="text">
    blaa blaa blaa
 </textarea>
</template>


<script>

//
export default {
  name: "text-output",
  props: {
    text:"blaa"
  },
  data: function () {
    return {
      count: 0,
    }
  },
}

</script>

