<?php

class CncdKoalectAction {
  public static function execute($action, $body, $expectedToken) {
    if (!$action) {
      throw new Exception('Error: action is missing', 400);
    }

    if ($action == 'getpayment') {
      $payment = new CncdKoalectPayment();
      return $payment->get($body, $expectedToken);
    }
    else {
      throw new Exception("Error: action $action is not implemented", 400);
    }
  }
}
