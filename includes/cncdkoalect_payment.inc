<?php

class CncdKoalectPayment {
  public function get($body, $expectedToken) {
    $this->assertBodyIsValid($body);
    $this->assertTokenIsValid($body, $expectedToken);

    $processedOrders = [];
    $orderCounter = 1;
    $contrib = new CncdKoalectContribution();

    foreach ($body->orders as $order) {
      $processedOrder = $this->validateAndCloneOrder($order, $orderCounter);
      $processedOrder->status = $contrib->getStatus($order->id, $order->reference, $order->amount);

      $processedOrders[] = $processedOrder;

      $orderCounter++;
    }

    return $processedOrders;
  }

  private function assertBodyIsValid($body) {
    if (!is_object($body)) {
      throw new Exception('Error: Request body is not an object', 400);
    }

    if (!property_exists($body, 'token')) {
      throw new Exception('Error: Request body is missing a token', 400);
    }

    if (!property_exists($body, 'orders')) {
      throw new Exception('Error: Request body is missing orders', 400);
    }

    if (!is_array($body->orders)) {
      throw new Exception('Error: The orders property is not an array', 400);
    }

    if (count($body->orders) == 0) {
      throw new Exception('Error: No orders found in the request', 400);
    }

    if (count($body->orders) > 1000) {
      throw new Exception('Error: Too many orders in the request', 400);
    }
  }

  private function assertTokenIsValid($body, $expectedToken) {
    if ($body->token <> $expectedToken) {
      throw new Exception('Error: invalid token', 403);
    }
  }

  private function validateAndCloneOrder($order, $orderCounter) {
    if (!property_exists($order, 'id')) {
      throw new Exception("Error: property 'id' is missing in order element $orderCounter", 403);
    }

    if (!property_exists($order, 'reference')) {
      throw new Exception("Error: property 'reference' is missing in order element $orderCounter", 403);
    }

    if (!property_exists($order, 'amount')) {
      throw new Exception("Error: property 'amount' is missing in order element $orderCounter", 403);
    }

    $o = new stdClass();
    $o->id = $order->id;
    $o->reference = $order->reference;
    $o->amount = $order->amount;
    $o->status = '';

    return $o;
  }
}
