<?php
// $Id: theme.inc,v 1.1.2.11 2009/11/18 04:15:39 yhahn Exp $

/**
 * Display feature component info
 */
function template_preprocess_features_admin_components(&$vars) {
  drupal_add_css(drupal_get_path('module', 'features') .'/features.css');
  $form = $vars['form'];

  // Basic info
  $vars['name'] = $form['#info']['name'];
  $vars['description'] = $form['#info']['description'];

  // Dependencies
  $rows = array();
  foreach ($form['#dependencies'] as $dependency => $status) {
    $rows[] = array(
      array('data' => $dependency, 'class' => 'component'),
      theme('features_module_status', $status),
    );
  }
  $vars['dependencies'] = theme('table', array(t('Dependency'), t('Status')), $rows);

  // Components
  $rows = array();
  if (!empty($form['#info']['features'])) {
    foreach ($form['#info']['features'] as $component => $items) {
      if (!empty($items)) {
        $header = $data = array();
        if (element_children($form['revert'])) {
          $header[] = array(
            'data' => isset($form['revert'][$component]) ? drupal_render($form['revert'][$component]) : '',
            'header' => TRUE
          );
        }
        $header[] = array(
          'data' => $component,
          'header' => TRUE
        );
        $header[] = array(
          'data' => drupal_render($form['components'][$component]),
          'header' => TRUE
        );
        $rows[] = $header;

        if (element_children($form['revert'])) {
          $data[] = '';
        }
        $data[] = array(
          'data' => theme('features_component_list', $items, $items),
          'colspan' => 2,
          'class' => 'component'
        );
        $rows[] = $data;
      }
    }
  }
  $vars['components'] = theme('table', array(), $rows);

  // Other elements
  $vars['buttons'] = drupal_render($form['buttons']);
  $vars['form'] = drupal_render($form);
}

/**
 * Preprocessor for theme('features_form').
 */
function template_preprocess_features_form(&$vars) {
  drupal_add_css(drupal_get_path('module', 'features') .'/features.css');
  drupal_add_js(drupal_get_path('module', 'features') .'/features.js');

  $first = TRUE;
  $package_links = array();
  foreach (element_children($vars['form']) as $package) {
    if (!empty($vars['form'][$package]['status'])) {
      $package_links[$package] = array(
        'title' => $vars['form'][$package]['#title'],
        'href' => $_GET['q'],
        'fragment' => $package,
      );
      $class = $first ? "$package features-package-active" : $package;
      $package_links[$package]['attributes'] = $first ? array('class' => 'features-package-active') : array();
      $vars['packages'][$class] = $vars['form'][$package];

      unset($vars['form'][$package]);
      $first = FALSE;
    }
  }
  $vars['package_links'] = $package_links;
}

/**
 * Themes a module status display.
 */
function theme_features_module_status($status, $module = NULL) {
  switch ($status) {
    case FEATURES_MODULE_ENABLED:
      $text = !empty($module) ? $module : t('Enabled');
      return "<span class='admin-enabled'>{$text}</span>";
    case FEATURES_MODULE_DISABLED:
      $text = !empty($module) ? $module : t('Disabled');
      return "<span class='admin-disabled'>{$text}</span>";
    case FEATURES_MODULE_MISSING:
      $text = !empty($module) ? $module : t('Missing');
      return "<span class='admin-missing'>{$text}</span>";
  }
}

/**
 * Themes a module status display.
 */
function theme_features_storage_link($storage, $path = NULL, $options = array()) {
  $classes = array(
    FEATURES_OVERRIDDEN => 'admin-overridden',
    FEATURES_DEFAULT => 'admin-default',
    FEATURES_NEEDS_REVIEW => 'admin-needs-review',
    FEATURES_REBUILDING => 'admin-rebuilding',
    FEATURES_REBUILDABLE => 'admin-rebuilding',
  );
  $text = array(
    FEATURES_OVERRIDDEN => t('Overridden'),
    FEATURES_DEFAULT => t('Default'),
    FEATURES_NEEDS_REVIEW => t('Needs review'),
    FEATURES_REBUILDING => t('Rebuilding'),
    FEATURES_REBUILDABLE => t('Rebuilding'),
  );
  if ($path) {
    $options['attributes']['class'] = $classes[$storage] . ' features-storage';
    return l($text[$storage], $path, $options);
  }
  else {
    return "<span class='{$classes[$storage]} features-storage'>{$text[$storage]}</span>";
  }
}

/**
 * Theme function for displaying form buttons
 */
function theme_features_form_buttons($element) {
  drupal_add_css(drupal_get_path('module', 'features') .'/features.css');

  $output = drupal_render($element);
  return !empty($output) ? "<div class='buttons clear-block'>{$output}</div>" : '';
}

/**
 * Theme for features management form.
 */
function theme_features_form_package($form) {
  drupal_add_css(drupal_get_path('module', 'features') .'/features.css');
  drupal_add_js(drupal_get_path('module', 'features') .'/features.js');

  $output = '';

  $header = array('', t('Feature'), t('Signature'));
  if (isset($form['state'])) {
    $header[] = t('State');
  }
  if (isset($form['actions'])) {
    $header[] = t('Actions');
  }

  $rows = array();
  foreach (element_children($form['status']) as $element) {
    // Yank title & description fields off the form element for
    // rendering in their own cells.
    $name = "<div class='feature'>";
    $name .= "<strong>{$form['status'][$element]['#title']}</strong>";
    $name .= "<div class='description'>{$form['status'][$element]['#description']}</div>";
    $name .= "</div>";
    unset($form['status'][$element]['#title']);
    unset($form['status'][$element]['#description']);

    // Determine row & cell classes
    $class = $form['status'][$element]['#default_value'] ? 'enabled' : 'disabled';

    $row = array(
      'status' => array('data' => drupal_render($form['status'][$element]), 'class' => 'status'),
      'name' => array('data' => $name, 'class' => 'name'),
      'sign' => array('data' => drupal_render($form['sign'][$element]), 'class' => 'sign'),
    );
    if (isset($form['state'])) {
      $row['state'] = array('data' => drupal_render($form['state'][$element]), 'class' => 'state');
    }
    if (isset($form['actions'])) {
      $row['actions'] = array('data' => drupal_render($form['actions'][$element]), 'class' => 'actions');
    }
    $rows[] = array('data' => $row, 'class' => $class);
  }

  if (empty($rows)) {
    $rows[] = array('', array('data' => t('No features available.'), 'colspan' => count($header)));
  }

  $class = count($header) > 3 ? 'features features-admin' : 'features features-manage';
  $output .= theme('table', $header, $rows, array('id' => 'features-form-table', 'class' => $class));

  // Prevent section from being rendered by drupal_render().
  unset($form['features']);

  $output .= drupal_render($form['buttons']);
  $output .= drupal_render($form);
  return $output;
}

/**
 * Theme functions ====================================================
 */

/**
 * Export selection / display for features export form.
 */
function theme_features_form_export($form) {
  drupal_add_css(drupal_get_path('module', 'features') .'/features.css');
  drupal_add_js(drupal_get_path('module', 'features') .'/features.js');

  $output = '';
  $output .= "<div class='clear-block features-components'>";
  $output .= "<div class='column'>". drupal_render($form['components']) . drupal_render($form['sources']) ."</div>";
  $output .= "<div class='column'>". drupal_render($form['features']) ."</div>";
  $output .= "</div>";
  $output .= drupal_render($form);
  return $output;
}

/**
 * Theme a set of features export components.
 */
function theme_features_form_components($form) {
  $output = '';
  foreach (element_children($form) as $key) {
    unset($form[$key]['#title']);
    $output .= "<div class='features-select features-select-{$key}'>". drupal_render($form[$key]) ."</div>";
  }
  return $output;
}

/**
 * Theme a set of features export components.
 */
function theme_features_components($info, $sources = array()) {
  $output = '';
  $rows = array();
  if (!empty($info['features']) || !empty($info['dependencies']) || !empty($sources)) {
    foreach ($info['features'] as $component => $items) {
      if (!empty($items) || !empty($sources[$component])) {
        $source_items = isset($sources[$component]) ? $sources[$component] : array();
        $rows[] = array(array('data' => $component, 'header' => TRUE));
        $rows[] = array(array('data' => theme('features_component_list', $items, $source_items), 'class' => 'component'));
      }
    }
    if (!empty($info['dependencies'])) {
      $source_items = isset($sources['dependencies']) ? $sources['dependencies'] : $info['dependencies'];
      $rows[] = array(array('data' => t('dependencies'), 'header' => TRUE));
      $rows[] = array(array('data' => theme('features_component_list', $info['dependencies'], $source_items), 'class' => 'component'));
    }
    $output .= theme('table', array(), $rows);
    $output .= theme('features_component_key');
  }
  return $output;
}

/**
 * Theme individual components in a component list.
 */
function theme_features_component_list($items, $source = array()) {
  $list = array();
  foreach ($items as $component) {
    // If component is not in source list, it was autodetected
    if (!in_array($component, $source)) {
      $list[] = "<span class='features-detected'>{$component}</span>";
    }
    else {
      $list[] = "<span class='features-source'>{$component}</span>";
    }
  }
  foreach ($source as $component) {
    // If a source component is no longer in the items, it was removed because
    // it is provided by a dependency.
    if (!in_array($component, $items)) {
      $list[] = "<span class='features-dependency'>{$component}</span>";
    }
  }
  return "<span class='features-component-list'>". implode(' ', $list) ."</span>";
}

/**
 * Provide a themed key for a component list.
 */
function theme_features_component_key() {
  $list = array();
  $list[] = "<span class='features-source'>" . t('Normal') . "</span>";
  $list[] = "<span class='features-detected'>" . t('Auto-detected') . "</span>";
  $list[] = "<span class='features-dependency'>" . t('Provided by dependency') . "</span>";
  return "<span class='features-component-list features-component-key'>". implode(' ', $list) ."</span>";
}
