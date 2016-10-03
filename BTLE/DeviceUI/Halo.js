
    //
    // Perform a non-blocking send of an event to the remote device.
    //
    function NonBlockingSend( type, payload )
    {
        $.ajax({ url: "http://localhost:8080/Send&type="+type+"?data="+payload,
            success: function(data)
            {
                //$("#div2").html('sddfsdf '+data);
            }
        });
    }



    //
    // All events received from the remote device end up here to be transformed
    // into js events.
    //
    function ProcessReceivedEvent( data )
    {
        if(data.type == 456)
        {
            text    = "Timestamp is: " + data.timestamp + ", type is: " + data.type + ", payload is: " + data.payload
            //$("#div1").html(text);
            $('#pie1').attr('transform','rotate('+data.payload+', 0.5, 0.5)');
            $('#pie2').attr('transform','rotate('+(data.payload+90)+', 0.5, 0.5)');
            $('#pie3').attr('transform','rotate('+(data.payload+180)+', 0.5, 0.5)');
            $('#pie4').attr('transform','rotate('+(data.payload+270)+', 0.5, 0.5)');
        }

        if(data.type == 345)
        {
            text    = "Timestamp is: " + data.timestamp + ", type is: " + data.type + ", payload is: " + data.payload
            $("#div2").html(text);
        }
    }


    //
    // Perform a blocking send of an event to the remote device.
    //
    function BlockingSend( type, payload, completion )
    {
        $.ajax({ url: "http://localhost:8080/SendAndReceive&type="+type+"?data="+payload,
            success: function(dataAsJSON)
            {
                data    = JSON.parse(dataAsJSON);
                completion(data);
            }
        });
    }


    //
    // Repeatedly long-poll for events. This is how the remote device can communicate
    // asynchronously with the js world.
    //
    (function PollForAsyncEvents()
    {
       $.ajax({ url: "http://localhost:8080/AsyncPoll&type=314159265?data=919212313",
            success: function(dataAsJSON)
            {
                data    = JSON.parse(dataAsJSON);
                ProcessReceivedEvent(data);
            },
            complete: function(jqXHR,status)
            {
                if(status == 'error')
                {
                    setTimeout( function() {PollForAsyncEvents();}, 1000 );
                }
                else
                {
                    PollForAsyncEvents();
                }
            },
       });

    })();




