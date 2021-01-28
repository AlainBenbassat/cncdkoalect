<?php

class CncdKoalectPayment {
  public function get($body, $expectedToken) {
    $this->assertBodyIsValid($body);
    $this->assertTokenIsValid($body, $expectedToken);

    $processedStocks = [];
    $stockCounter = 1;
    $contrib = new CncdKoalectContribution();

    foreach ($body->stocks as $stock) {
      $processedStock = $this->validateAndCloneStock($stock, $stockCounter);
      $processedStock->status = $contrib->getStatus($stock->id, $stock->reference, $stock->amount);

      $processedStocks[] = $processedStock;

      $stockCounter++;
    }

    return $processedStocks;
  }

  private function assertBodyIsValid($body) {
    if (!is_object($body)) {
      throw new Exception('Error: Request body is not an object', 400);
    }

    if (!property_exists($body, 'token')) {
      throw new Exception('Error: Request body is missing a token', 400);
    }

    if (!property_exists($body, 'stocks')) {
      throw new Exception('Error: Request body is missing stocks', 400);
    }

    if (!is_array($body->stocks)) {
      throw new Exception('Error: The stocks property is not an array', 400);
    }

    if (count($body->stocks) == 0) {
      throw new Exception('Error: No stocks found in the request', 400);
    }

    if (count($body->stocks) > 1000) {
      throw new Exception('Error: Too many stocks in the request', 400);
    }
  }

  private function assertTokenIsValid($body, $expectedToken) {
    if ($body->token <> $expectedToken) {
      throw new Exception('Error: invalid token', 403);
    }
  }

  private function validateAndCloneStock($stock, $stockCounter) {
    if (!property_exists($stock, 'id')) {
      throw new Exception("Error: property 'id' is missing in stock element $stockCounter", 403);
    }

    if (!property_exists($stock, 'reference')) {
      throw new Exception("Error: property 'reference' is missing in stock element $stockCounter", 403);
    }

    if (!property_exists($stock, 'amount')) {
      throw new Exception("Error: property 'amount' is missing in stock element $stockCounter", 403);
    }

    $o = new stdClass();
    $o->id = $stock->id;
    $o->reference = $stock->reference;
    $o->amount = $stock->amount;
    $o->status = '';

    return $o;
  }
}
