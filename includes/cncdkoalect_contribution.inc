<?php

class CncdKoalectContribution {
  public function __construct() {
    civicrm_initialize();
  }

  public function getStatus($externalId, $source, $amount) {
    $total = $this->findBySource($source);
    if (!$total) {
      $total = $this->findBySourceWithTextAfter($source);
    }

    return $this->amountCompare($total, $amount);
  }

  private function findBySource($source) {
    $sql = "
      select
        ifnull(sum(total_amount) * 100, 0)
      from
        civicrm_contribution
      where
        source like %1
      and
        financial_type_id = 18
    ";
    $sqlParams = [
      1 => ["% $source", 'String'],
    ];

    return CRM_Core_DAO::singleValueQuery($sql, $sqlParams);
  }

  private function findBySourceWithTextAfter($source) {
    $sql = "
      select
        ifnull(sum(total_amount) * 100, 0)
      from
        civicrm_contribution
      where
        source like %1
      and
        financial_type_id = 18
    ";
    $sqlParams = [
      1 => ["% $source %", 'String'],
    ];

    return CRM_Core_DAO::singleValueQuery($sql, $sqlParams);
  }

  private function amountCompare($foundAmount, $expectedAmount) {
    if ($foundAmount == 0){
      return 'En attente';
    }
    elseif ($foundAmount == $expectedAmount) {
      return 'Validé';
    }
    elseif ($foundAmount < $expectedAmount) {
      return 'Incomplet';
    }
    elseif ($foundAmount > $expectedAmount) {
      return 'A vérifier';
    }
    else {
      return 'Erreur';
    }
  }
}
