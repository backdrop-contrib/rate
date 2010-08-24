<?php
/**
 * @file
 * Rate widget theme
 *
 * This is the default template for rate widgets. See section 3 of the README
 * file for information on theming widgets.
 *
 */

foreach ($links as $link) {
  print $link['content'];
}

print $results['count'];

?>