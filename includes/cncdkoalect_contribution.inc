<?php

class CncdKoalectContribution {
  public function __construct() {
    civicrm_initialize();
  }

  public function getStatus($externalId, $source, $amount) {
    $total = $this->findBySource($source);

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

  private function amountCompare($foundAmount, $expectedAmount) {
    if ($foundAmount == 0){
      return 'en attente';
    }
    elseif ($foundAmount == $expectedAmount) {
      return 'paiement validé';
    }
    elseif ($foundAmount < $expectedAmount) {
      return 'paiement incomplet';
    }
    elseif ($foundAmount > $expectedAmount) {
      return 'erreur';
    }
    else {
      return 'erreur';
    }
  }
}
