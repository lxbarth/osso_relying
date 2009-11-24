/**
 * @file
 * Javascript behaviors for OpenID SSO.
 */

Drupal.behaviors.openid_sso = function() {

  // Submit auto-submit forms.
  $('.auto-submit').submit();

  // Hide submit button and submit form, hide noscript content, show script
  // content and submit.
  $('.auto-submit input:submit').hide();
  $('.auto-submit .noscript').hide();
  $('.auto-submit .script').show();
}
