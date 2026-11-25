#include "composition_over_inheritance.hpp"
#include <cassert>
#include <iostream>

int main() {
    bad::EmailNotifier bad_notifier;
    bad_notifier.send("alice@example.com");

    good::Logger logger;
    good::EmailNotifier good_notifier(logger);
    good_notifier.send("alice@example.com");

    good::SmsNotifier sms(logger);
    sms.send("+123456789");

    std::cout << "Composition over Inheritance: flexible delegation\n";
    return 0;
}