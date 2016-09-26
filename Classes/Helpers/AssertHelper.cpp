//
//  AssertHelper.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/09/27.
//
//

#include "Helpers/AssertHelper.h"

#include "Utils/AssertUtils.h"

bool AssertHelper::init()
{
    return true;
}

#pragma mark -
#pragma mark Text

// テキストの1行をstackにpush
AssertHelper* AssertHelper::pushTextLine(const string &text)
{
    this->textStack.push_back(text);
    return this;
}

// テキストの1行を"key: value"形式でstackにpush
AssertHelper* AssertHelper::pushTextLineKeyValue(const string &key, const string &value)
{
    this->pushTextLine(key + ": " + value);
    return this;
}

// テキストの最後のラインをstackからpop
AssertHelper* AssertHelper::popTextLine()
{
    this->textStack.pop_back();
    return this;
}

// テキストの最後のラインをstackから指定回数popする
AssertHelper* AssertHelper::popTextLines(const int count)
{
    for (int i = 0; i < count; i++) {
        this->popTextLine();
    }
    return this;
}


#pragma mark -
#pragma mark DisplayAssertion

// WarningAssertを出す
void AssertHelper::warningAssert(const string& title)
{
    LastSupper::AssertUtils::warningAssert(this->generateTextFromStack(title));
}

// fatalAssertを出す
void AssertHelper::fatalAssert(const string& title)
{
    LastSupper::AssertUtils::fatalAssert(this->generateTextFromStack(title));
}


#pragma mark -
#pragma mark PrivateMethods

// タイトルとテキストスタックからAssertMessageを生成
string AssertHelper::generateTextFromStack(const string &title)
{
    string text = title;
    for (string textLine : this->textStack) {
        text += "\n" + textLine;
    }
    return text;
}
