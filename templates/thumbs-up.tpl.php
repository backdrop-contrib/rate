<?php
/**
 * @file
 * Rate widget theme
 *
 * This is the default template for rate widgets. See section 3 of the README
 * file for information on theming widgets.
 */

if ($results['user_vote']) {
  $info = format_plural($results['count'], 'Only you voted.', '@count users have voted, including you.');
}
else {
  $info = format_plural($results['count'], '@count user has voted.', '@count users have voted.');
}

print theme('rate_button', $links[0]['text'], $links[0]['href'], 'rate-thumbs-up-btn-up');

print '<div class="rate-info">' . $info . '</div>';

?>