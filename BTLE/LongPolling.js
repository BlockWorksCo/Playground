


(function poll() {
   setTimeout(function() {
       $.ajax({ url: "server", success: function(data) {
            sales.setValue(data.value);
       }, dataType: "json", complete: poll });
    }, 30000);
})();
