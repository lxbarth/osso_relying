<?php

/**
 * Helper to implementation of hook_user_default_permissions().
 */
function _simple_blog_user_default_permissions() {
  $permissions = array();

  // Exported permission: create story content
  $permissions[] = array(
    'name' => 'create story content',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: delete own story content
  $permissions[] = array(
    'name' => 'delete own story content',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: edit own story content
  $permissions[] = array(
    'name' => 'edit own story content',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  return $permissions;
}