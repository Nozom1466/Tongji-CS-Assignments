#include <string>

class Account {
public:
    Account(const std::string& cardNo, long initBalance);

    std::string getCardNo() const;
    long getBalance() const;

    void debit(long amount);
    void credit(long amount);
    
private:
    std::string cardNo;
    long balance;
};



#include "Account.h"

Account::Account(const std::string& cardNo, long initBalance): cardNo(cardNo), balance(initBalance) {}

std::string Account::getCardNo() const {
    return cardNo;
}

long Account::getBalance() const {
    return balance;
}

void Account::debit(long amount) {
    balance -= amount;
}

void Account::credit(long amount) {
    balance += amount;
}



#include "Account.h"
#include "AccountManagement.h"

class AccountService {
public:
    AccountService(AccountManagement& accountMgmt);

    void withdraw(const std::string& cardNo, long amount);
    void deposit(const std::string& cardNo, long amount);
    void transfer(const std::string& fromCardNo, const std::string& toCardNo, long amount);
    long inquiry(const std::string& cardNo);

private:
    AccountManagement& accountMgmt;
};


#include "AccountService.h"

AccountService::AccountService(AccountManagement& accountMgmt): accountMgmt(accountMgmt) {}

void AccountService::withdraw(const std::string& cardNo, long amount) {
    Account account = accountMgmt.findAccount(cardNo);
    account.debit(amount);
    accountMgmt.updateAccount(account);
}

void AccountService::deposit(const std::string& cardNo, long amount) {
    Account account = accountMgmt.findAccount(cardNo);
    account.credit(amount);
    accountMgmt.updateAccount(account);
}

void AccountService::transfer(const std::string& fromCardNo, const std::string& toCardNo, long amount) {
    Account fromAccount = accountMgmt.findAccount(fromCardNo);
    Account toAccount = accountMgmt.findAccount(toCardNo);
    fromAccount.debit(amount);
    toAccount.credit(amount);
    accountMgmt.updateAccount(fromAccount);
    accountMgmt.updateAccount(toAccount);
}

long AccountService::inquiry(const std::string& cardNo) {
    Account account = accountMgmt.findAccount(cardNo);
    return account.getBalance();
}


#include "Account.h"
#include <string>

class AccountManagement {
public:
    virtual Account findAccount(const std::string& userID) = 0;
    virtual void updateAccount(const Account& account) = 0;
};

#include "gtest/gtest.h"
#include "Account.h"

TEST(AccountTest, Initialization) {
    Account account("12345", 1000);
    EXPECT_EQ(account.getCardNo(), "12345");
    EXPECT_EQ(account.getBalance(), 1000);
}

TEST(AccountTest, Debit) {
    Account account("12345", 1000);
    account.debit(200);
    EXPECT_EQ(account.getBalance(), 800);
}

TEST(AccountTest, Credit) {
    Account account("12345", 1000);
    account.credit(200);
    EXPECT_EQ(account.getBalance(), 1200);
}


#include "gtest/gtest.h"
#include "AccountService.h"
#include "AccountManagement.h"

class MockAccountManagement : public AccountManagement {
public:
    MOCK_METHOD1(findAccount, Account(const std::string&));
    MOCK_METHOD1(updateAccount, void(const Account&));
};

TEST(AccountServiceTest, Inquiry) {
    MockAccountManagement mockAccountMgmt;
    Account account("12345", 1000);

    EXPECT_CALL(mockAccountMgmt, findAccount("12345"))
    .WillOnce(testing::Return(account));

    AccountService service(mockAccountMgmt);
    long balance = service.inquiry("12345");

    EXPECT_EQ(balance, 1000);
}

TEST(AccountServiceTest, Withdraw) {
    MockAccountManagement mockAccountMgmt;
    Account account("12345", 1000);

    EXPECT_CALL(mockAccountMgmt, findAccount("12345"))
    .WillOnce(testing::Return(account));
    EXPECT_CALL(mockAccountMgmt, updateAccount(testing::An<const Account&>()));

    AccountService service(mockAccountMgmt);
    service.withdraw("12345", 200);

    EXPECT_EQ(account.getBalance(), 800);
}

TEST(AccountServiceTest, Deposit) {
    MockAccountManagement mockAccountMgmt;
    Account account("12345", 1000);

    EXPECT_CALL(mockAccountMgmt, findAccount("12345"))
    .WillOnce(testing::Return(account));
    EXPECT_CALL(mockAccountMgmt, updateAccount(testing::An<const Account&>()));

    AccountService service(mockAccountMgmt);
    service.deposit("12345", 200);

    EXPECT_EQ(account.getBalance(), 1200);
}

TEST(AccountServiceTest, Transfer) {
    MockAccountManagement mockAccountMgmt;
    Account fromAccount("12345", 1000);
    Account toAccount("67890", 500);

    EXPECT_CALL(mockAccountMgmt, findAccount("12345"))
    .WillOnce(testing::Return(fromAccount));
    EXPECT_CALL(mockAccountMgmt, findAccount("67890"))
    .WillOnce(testing::Return(toAccount));
    EXPECT_CALL(mockAccountMgmt, updateAccount(testing::An<const Account&>())).Times(2);

    AccountService service(mockAccountMgmt);
    service.transfer("12345", "67890", 300);

    EXPECT_EQ(fromAccount.getBalance(), 700);
    EXPECT_EQ(toAccount.getBalance(), 800);
}

void AccountService::transfer(const std::string& fromCardNo, const std::string& toCardNo, long amount) {
    if (amount > 3000) {
        throw std::invalid_argument("Transfer amount exceeds the limit of 3000.");
    }

    Account fromAccount = accountMgmt.findAccount(fromCardNo);
    Account toAccount = accountMgmt.findAccount(toCardNo);
    fromAccount.debit(amount);
    toAccount.credit(amount);
    accountMgmt.updateAccount(fromAccount);
    accountMgmt.updateAccount(toAccount);
}

TEST(AccountServiceTest, TransferExceedsLimit) {
    MockAccountManagement mockAccountMgmt;
    Account fromAccount("12345", 1000);
    Account toAccount("67890", 500);

    EXPECT_CALL(mockAccountMgmt, findAccount("12345"))
    .WillOnce(testing::Return(fromAccount));
    EXPECT_CALL(mockAccountMgmt, findAccount("67890"))
    .WillOnce(testing::Return(toAccount));

    AccountService service(mockAccountMgmt);

    EXPECT_THROW(service.transfer("12345", "67890", 3500), std::invalid_argument);
}

