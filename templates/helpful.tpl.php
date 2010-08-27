<?php
/**
 * @file
 * Rate widget theme
 *
 * This is the default template for rate widgets. See section 3 of the README
 * file for information on theming widgets.
 *
 */

if ($results['user_vote']) {
  $info = t('You voted \'@option\'.', array('@option' => $results['user_vote']));
}

?>

<?php foreach ($links as $link): ?>
  <?php print theme('rate_button', $link['text'], $link['href'], 'rate-helpful-btn'); ?>
  <div class="rate-helpful-votes"><?php print $link['votes']; ?></div>
<?php endforeach; ?>

<?php
if (!empty($info)) {
  print '<div class="rate-info">' . $info . '</div>';
}
?>