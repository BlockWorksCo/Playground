

<template>
 <textarea v-model="text" cols=80 rows=25 maxlength=2000 readonly>
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

