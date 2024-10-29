
// Initialize ports for L293D control
void initMotor() {
    DDRD |= (1 << PD2) | (1 << PD3); // Set PD2 and PD3 as output
}

void rotateMotorClockwise() {
    PORTD |= (1 << PD2);
    PORTD &= ~(1 << PD3);
}

void rotateMotorCounterClockwise() {
    PORTD |= (1 << PD3);
    PORTD &= ~(1 << PD2);
}

void stopMotor() {
    PORTD &= ~(1 << PD2);
    PORTD &= ~(1 << PD3);
}
