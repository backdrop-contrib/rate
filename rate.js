Drupal.behaviors.rate = function(context) {
  $('a.rate-button', context).click(function() {
    
    // Reveal the widget id.
    $('#' + this.id).parents('div.rate-widget').each(function() {
      widget = this.id;
    });

    // Extract required variables using regexp.
    ids = this.getAttribute('href').match(/rate\=([a-f0-9]{32})/);
    token = ids[1];
    ids = widget.match(/^rate\-([a-z]+)\-([0-9]+)\-([0-9]+)$/);
    content_type = ids[1];
    content_id = ids[2];
    widget_id = ids[3];

    // Request new widget HTML.
    $.get('/rate/vote/js?widget_id=' + widget_id + '&content_type=' + content_type + '&content_id=' + content_id + '&token=' + token, function(data) {
      $('#' + widget).html(data);
      Drupal.behaviors.rate($('#' + widget));
    });

    return false;
  });
}
