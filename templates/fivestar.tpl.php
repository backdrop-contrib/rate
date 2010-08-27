<?php
/**
 * @file
 * Rate widget theme
 *
 * This is the default template for rate widgets. See section 3 of the README
 * file for information on theming widgets.
 *
 */

// Calculate rating in number of stars.
$rating = round($results['rating'] / 25);

// Generate the info line.
$info = '';
if (isset($results['user_vote'])) {
  $vote = round($results['user_vote'] / 25) + 1;
  $info .= t('You voted !vote.', array('!vote' => $vote)) . ' ';
}
$info .= t('Total votes: !count', array('!count' => $results['count']));
$info = filter_xss($info);

for ($i = 0; $i < 5; $i++) {
  if (round($results['rating'] / 25) >= $i) {
    $class = 'rate-fivestar-btn-filled';
  }
  else {
    $class = 'rate-fivestar-btn-empty';
  }
  print theme('rate_button', $links[$i]['text'], $links[$i]['href'], $class);
}

print '<div class="rate-info">' . $info . '</div>';

?>