Drupal.behaviors.RateSlider = function(context) {
  $('.rate-widget-slider:not(.rate-slider-processed)',context).addClass('rate-slider-processed').each(function() {
    var widget = $(this);
    var ids = widget.attr('id').match(/^rate\-([a-z]+)\-([0-9]+)\-([0-9]+)\-([0-9])$/);
    var data = {
      content_type: ids[1],
      content_id: ids[2],
      widget_id: ids[3],
      widget_mode: ids[4]
    };

    var s = $(".rate-slider", widget);
    var v = $(s).attr("class").match(/rate\-value\-([0-9]+)/)[1];
    
    widget.prepend(s);

    // Check if this widget is active (disabled widgets have <span>'s instead of <a>'s).'
    if ($("ul a", widget).length > 0) {
      // Add the slider.
      s.slider({
        min: 0,
        max: 100,
        steps: 100,
        startValue: v, // jQuery UI 1.2
        value: v, // jQuery UI 1.3
        slide: function(event,ui) {
          //ui.value;
          $(".rate-slider-value", s).width(ui.value + '%');
        },
        stop: function(event,ui) {
          data.value = ui.value;
          return Drupal.rateVote(widget, data, '');
        }
      });
    }
    else {
      // Widget is disabled. Only add the slider styling.
      $(s).width('200px');
      $(s).addClass('ui-slider');
    }

    // Add the rating bar.
    s.prepend('<div class="rate-slider-value" style="width: ' + v + '%" />');

    // Hide the links for the non-js variant.
    $("ul", widget).hide();
  });
};
