function makesvg(percentage, inner_text=""){

  var abs_percentage = Math.abs(percentage).toString();
  var percentage_str = percentage.toString();
  var classes = ""

  if(percentage > 0 && percentage <= 40){
    classes = "danger-stroke";
  } else if(percentage > 40 && percentage <= 80){
    classes = "warning-stroke";
  } else{
    classes = "success-stroke";
  }

 var svg = '<svg class="circle-chart" viewbox="0 0 33.83098862 33.83098862" xmlns="http://www.w3.org/2000/svg">'
     + '<circle class="circle-chart__background" cx="16.9" cy="16.9" r="15.9" />'
     + '<circle class="circle-chart__circle '+classes+'"'
     + 'stroke-dasharray="'+ abs_percentage+',100"    cx="16.9" cy="16.9" r="15.9" />'
     + '<g class="circle-chart__info">'
     + '   <text class="circle-chart__percent" x="17.9" y="15.5">'+percentage_str+'%</text>';

  if(inner_text){
    svg += '<text class="circle-chart__subline" x="16.91549431" y="22">'+inner_text+'</text>'
  }
  
  svg += ' </g></svg>';
  
  return svg
}

(function( $ ) {

    $.fn.circlechart = function() {
        if ($('#filename').text() == "") {
            chartss = $('#chart-box').detach();
            $('#chart-box-in').append($("<div class='col-lg-12 mb-5' id='before-detect' style='text-align: center'>" + "<h1>No Data</h1>" + "</div>"));
        } else {
            this.each(function () {
                var percentage = $(this).data("percentage");
                var inner_text = $(this).text();
                $(this).html(makesvg(percentage, inner_text));

            });
            fn.circlechart = function() {
                if($('#filename').text() == "") {
                    chartss = $('#chart-box').detach()
                }
            }
        };
        return this;
    };

}( jQuery ));