

OpenID SSO
==========

OpenID based Simple Sign-On among trusted sites.

Add a trusted Drupal OpenID provider to a Drupal site. Users enjoy a simplified
sign on process between trusted sites.

Assumes that the OpenID Provider in place is a Drupal site with OpenID Provider
SSO module enabled.

Note: this module does not provide true *Single* Sign-On functionality but a
largely simplified sign on experience for webs of trusted sites. Hence the name
*Simple* Sign-On.

Restrictions
============

OpenID SSO assumes that *every* user on the site will register or is already
registered on a trusted OpenID Provider with OpenID Provider SSO module enabled.

It will shut down any other means of logging on to the site by overriding user/
menu callbacks.

Thus, if there are existing users on your site (e. g. user 1 after installation)
you will have to make sure that these users have accounts on the OpenID Provider
and that their OpenIDs are added to their accounts on this site:

1) Go to OpenID Provider and create account for existing user. If account
   already exists, make OpenID has the format
   http://example.com/openid/[UserName]
2) On this site, add the user's OpenID to her account.
3) Now you should be able to log on on this site with [UserName] on user/

Installation
============

Install this module and dependencies.

In your site's settings.php file, define a trusted OpenID Provider. OpenID
Provider must have OpenID Provider SSO enabled.

$conf['openid_sso_provider'] = array(
  'url' => 'http://localhost/op', // No trailing slashes.
  'name' => 'SSO Web', // Mostly always the Provider's site name.
);


