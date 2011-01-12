// $Id$
(function ($) {
  Drupal.behaviors.rate = {
    attach: function(context) {
      $('.rate-widget:not(.rate-processed)', context).addClass('rate-processed').each(function () {
        var widget = $(this);
        var ids = widget.attr('id').match(/^rate\-([a-z]+)\-([0-9]+)\-([0-9]+)\-([0-9])$/);
        var data = {
          content_type: ids[1],
          content_id: ids[2],
          widget_id: ids[3],
          widget_mode: ids[4]
        };

        $('a.rate-button', widget).click(function() {
          var token = this.getAttribute('href').match(/rate\=([a-zA-Z0-9\-_]{32,64})/)[1];

          $.get(Drupal.settings.basePath + 'rate/vote/js?widget_id=' + data.widget_id + '&content_type=' + data.content_type + '&content_id=' + data.content_id + '&widget_mode=' + data.widget_mode + '&token=' + token + '&destination=' + escape(document.location), function(data) {
            if (data.match(/^https?\:\/\/[^\/]+\/(.*)$/)) {
              // We got a redirect.
              document.location = data;
            }
            else {
              // get parent object
              var p = widget.parent();

              widget.before(data);

              // remove widget
              widget.remove();
              widget = undefined;

              Drupal.attachBehaviors(p.get(0));
            }
          });

          return false;
        });
      });
    }
  };
})(jQuery);