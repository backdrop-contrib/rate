<?php

namespace Drupal\rate;

use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\DependencyInjection\ContainerInjectionInterface;
use Drupal\Core\Database\Connection;
use Drupal\Core\Session\AccountProxyInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Returns responses for Rate routes.
 */
class RateVote implements ContainerInjectionInterface {

  /**
   * The entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected $entityTypeManager;

  /**
   * Database connection object.
   *
   * @var \Drupal\Core\Database\Connection
   */
  protected $database;

  /**
   * Database connection object.
   *
   * @var \Drupal\rate\RateBotDetector
   */
  protected $botDetector;

  /**
   * Account proxy (the current user).
   *
   * @var \Drupal\Core\Session\AccountProxyInterface
   */
  protected $accountProxy;

  /**
   * Constructor for vote service.
   *
   * @param \Drupal\Core\Entity\EntityTypeManagerInterface $entity_type_manager
   *   The entity type manager.
   * @param \Drupal\Core\Database\Connection $database
   *   The entity type manager.
   * @param \Drupal\rate\RateBotDetector $bot_detector
   *   The bot detector service.
   * @param \Drupal\Core\Session\AccountProxyInterface $account_proxy
   *   The current user.
   */
  public function __construct(EntityTypeManagerInterface $entity_type_manager,
                              Connection $database,
                              RateBotDetector $bot_detector,
                              AccountProxyInterface $account_proxy) {
    $this->entityTypeManager = $entity_type_manager;
    $this->database = $database;
    $this->botDetector = $bot_detector;
    $this->accountProxy = $account_proxy;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('entity_type.manager'),
      $container->get('database'),
      $container->get('rate.bot_detector'),
      $container->get('current_user')
    );
  }

  /**
   * Record a vote.
   *
   * @param string $entity_type_id
   *   Entity type ID such as node.
   * @param string $vote_type_id
   *   Vote type id.
   * @param int $entity_id
   *   Entity id of the entity type.
   * @param bool $show_messages
   *   If TRUE, standard Drupal message will be set.
   */
  public function vote($entity_type_id, $vote_type_id, $entity_id, $show_messages = TRUE) {
    $entity = $this->entityTypeManager->getStorage($entity_type_id)->load($entity_id);
    $is_bot_vote = $this->botDetector->checkIsBot();

    if (!$is_bot_vote && $this->accountProxy->hasPermission('cast rate vote on ' . $entity_type_id . ' of ' . $entity->bundle())) {
      $vote_storage = $this->entityTypeManager->getStorage('vote');
      $vote_ids = $vote_storage->getUserVotes(
        $this->accountProxy->id(),
        $vote_type_id,
        $entity_type_id,
        $entity_id
      );

      // If user hasn't voted, save the vote.
      if (empty($vote_ids)) {
        $vote_type = $this->entityTypeManager->getStorage('vote_type')->load($vote_type_id);
        $vote = $this->entityTypeManager->getStorage('vote')->create(['type' => $vote_type_id]);
        $vote->setVotedEntityId($entity_id);
        $vote->setVotedEntityType($entity_type_id);
        $vote->setValueType($vote_type->getValueType());
        $vote->setValue(1);
        $vote->save();

        if ($show_messages) {
          drupal_set_message(t('Your :type vote was added.', [
            ':type' => $vote_type_id,
          ]));
        }
      }
      // Otherwise, inform user of previous vote.
      elseif ($show_messages) {
        drupal_set_message(
          t('You are not allowed to vote the same way multiple times.'), 'warning'
        );
      }
    }
  }

  /**
   * Record a vote.
   *
   * @param string $entity_type_id
   *   Entity type ID such as node.
   * @param int $entity_id
   *   Entity id of the entity type.
   * @param bool $show_messages
   *   If TRUE, standard Drupal message will be set.
   */
  public function undoVote($entity_type_id, $entity_id, $show_messages = TRUE) {
    $entity = $this->entityTypeManager->getStorage($entity_type_id)->load($entity_id);
    $is_bot_vote = $this->botDetector->checkIsBot();

    if (!$is_bot_vote && $this->accountProxy->hasPermission('cast rate vote on ' . $entity_type_id . ' of ' . $entity->bundle())) {
      $vote_storage = $this->entityTypeManager->getStorage('vote');
      $vote_ids = $vote_storage->getUserVotes(
        $this->accountProxy->id(),
        NULL,
        $entity_type_id,
        $entity_id
      );

      // If a vote has been found, remove it.
      if (!empty($show_messages)) {
        $vote = $vote_storage->load(array_pop($vote_ids));
        $vote->delete();
      }
      // Otherwise, inform user of previous vote.
      elseif ($show_messages) {
        drupal_set_message(
          t('A previous vote was not found.'), 'warning'
        );
      }
    }
  }

}
