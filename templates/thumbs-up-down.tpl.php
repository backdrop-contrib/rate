<?php
/**
 * @file
 * Rate widget theme
 *
 * This is the default template for rate widgets. See section 3 of the README
 * file for information on theming widgets.
 */

if ($results['user_vote']) {
  $info = t('You voted \'@option\'.', array('@option' => $results['user_vote'] == 1 ? t('up') : t('down')));
}

?>
<div class="rate-thumbs-up-down-rating-up"><?php print $results['up_percent'] . '%'; ?></div>
<?php
print theme('rate_button', $links[0]['text'], $links[0]['href'], 'rate-thumbs-up-down-btn-up');
print theme('rate_button', $links[1]['text'], $links[1]['href'], 'rate-thumbs-up-down-btn-down');
?>
<div class="rate-thumbs-up-down-rating-down"><?php print $results['down_percent'] . '%'; ?></div>

<?php
if (!empty($info)) {
  print '<div class="rate-info">' . $info . '</div>';
}
?>