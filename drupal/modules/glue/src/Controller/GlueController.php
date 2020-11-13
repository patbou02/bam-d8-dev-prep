<?php

/**
 * @file
 * Contains \Drupal\glue\Controller\GlueController.
 */

namespace Drupal\glue\Controller;

/**
 * Controller routines for glue page routes.
 */
class GlueController {
  public function helloWorldPage() {
    return array(
      '#markup' => t('<p>Hello, world!</p>'),
    );
  }
}
