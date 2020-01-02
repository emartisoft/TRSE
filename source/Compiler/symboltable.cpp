/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

#include "symboltable.h"
#include <qmath.h>

bool SymbolTable::isInitialized = false;
int SymbolTable::m_currentSid = 0;
QMap<QString, Symbol*> SymbolTable::m_constants;

void SymbolTable::ExitProcedureScope(bool removeSymbols) {
    // "TRUE" doesn't work
    if (removeSymbols)
    for (QString s: m_symbols.keys()) {
        if (s.startsWith(m_currentProcedure)) {
            m_symbols.remove(s);
        }
    }


    m_currentProcedure="";
}

SymbolTable::SymbolTable()
{
    m_currentSid = 0;
    InitBuiltins();
    Initialize();

}

void SymbolTable::DefineSid(unsigned int initAddress, unsigned int playAddress) {
    m_currentSid++;
    QString s = QString::number(m_currentSid);

    QString init = "SIDFILE_"+s+"_INIT";
    QString play = "SIDFILE_"+s+"_PLAY";
    m_constants[init] = new Symbol("$"+QString::number(initAddress,16),"ADDRESS", initAddress);
    m_constants[play] = new Symbol("$"+QString::number(playAddress,16),"ADDRESS", playAddress);


}

void SymbolTable::Initialize()
{
  //  if (isInitialized)
//        return;

  //  m_constants.clear();

    QString currentSystem = AbstractSystem::StringFromSystem(Syntax::s.m_currentSystem->m_system).toLower();
//    qDebug() << currentSystem;
    for (QString s: Syntax::s.m_syntaxData.split('\n')) {
        s= s.simplified();
        if (s.count()==0) continue;
        if (s.startsWith("#")) continue;
        s=s.replace(" ", "");

        QStringList data = s.split(";");
        if (data[0].toLower()!="c")
            continue;
        QString constant = data[1].toUpper();
        QString system = data[2].toLower();
        QString type = data[3].toLower();
        QStringList values = data[4].replace(" ","").split(",");
        QString value;
        if (Syntax::s.m_currentSystem->m_memoryType<values.count())
            value = values[Syntax::s.m_currentSystem->m_memoryType];
        else
            value = values[0];

        if (system.contains(currentSystem)) {
            int ival = Util::NumberFromStringHex(value);
            if (type=="b")
                m_constants[constant] = new Symbol(value,"BYTE", ival);
            if (type=="a")
                m_constants[constant] = new Symbol("^"+value,"ADDRESS", ival);
            if (type=="i")
                m_constants[constant] = new Symbol(value,"INTEGER", ival);


//            reservedWords.append(Token(TokenType::getType(word), word.toUpper()));
        }

     }
    if (!m_constants.contains("SIDFILE_1_INIT")) {
        m_constants["SIDFILE_1_INIT"] = new Symbol("","ADDRESS", 0);
      m_constants["SIDFILE_2_INIT"] = new Symbol("","ADDRESS", 0);
      m_constants["SIDFILE_1_PLAY"] = new Symbol("","ADDRESS", 0);
      m_constants["SIDFILE_2_PLAY"] = new Symbol("","ADDRESS", 0);
    }

    for (unsigned char key: Syntax::s.m_c64keys.keys()) {
        C64Key k = Syntax::s.m_c64keys[key];
        m_constants[k.m_key] = new Symbol(QString::number(k.m_value), "BYTE",  k.m_value);
    }

}

void SymbolTable::Define(Symbol *s, bool isUsed) {
    m_symbols[m_currentProcedure+ s->m_name] = s;
    m_symbols[m_currentProcedure+ s->m_name]->isUsed = isUsed;
}

void SymbolTable::Delete() {
    for (QString val : m_symbols.keys()) {
        Symbol* s = m_symbols[val];
        if (s!=nullptr) {
            if (s->m_value)
                delete s->m_value;

        }
        delete s;
    }

    // Delete static constants as well
    if (isInitialized) {
        for (QString val : m_constants.keys()) {
            Symbol* s = m_symbols[val];

            if (s!=nullptr) {
                if (s->m_value)
                    delete s->m_value;

            }
            delete s;
        }
        isInitialized = false;

    }
}

void SymbolTable::setName(QString s) {
    m_name = s;
}

void SymbolTable::InitBuiltins()
{

    Define(new BuiltInTypeSymbol("INTEGER",""));
    Define(new BuiltInTypeSymbol("WORD",""));
    Define(new BuiltInTypeSymbol("LONG",""));
    Define(new BuiltInTypeSymbol("REAL",""));
    Define(new BuiltInTypeSymbol("BYTE",""));
    Define(new BuiltInTypeSymbol("STRING",""));
    Define(new BuiltInTypeSymbol("CSTRING",""));
    Define(new BuiltInTypeSymbol("POINTER",""));
    Define(new BuiltInTypeSymbol("ARRAY",""));
    Define(new BuiltInTypeSymbol("INCBIN",""));
    Define(new BuiltInTypeSymbol("INCSID",""));
    Define(new BuiltInTypeSymbol("INCNSF",""));

    Define(new Symbol("return",""));

    Define(new Symbol("sine", "address"));
    Define(new Symbol("log2_table", "address"));
    Define(new Symbol("joystickup", "byte"));
    Define(new Symbol("joystickdown", "byte"));
    Define(new Symbol("joystickleft", "byte"));
    Define(new Symbol("joystickright", "byte"));
    Define(new Symbol("joystickbutton", "byte"));

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C64 ||
            Syntax::s.m_currentSystem->m_system==AbstractSystem::VIC20) {

        Define(new Symbol("joy1", "byte"));
        Define(new Symbol("joy1pressed", "byte"));
    }
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C64) {

        Define(new Symbol("joy2", "byte"));
        Define(new Symbol("joy2pressed", "byte"));
    }

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C64 ||
            Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4 ||
            Syntax::s.m_currentSystem->m_system==AbstractSystem::C128 ||
            Syntax::s.m_currentSystem->m_system==AbstractSystem::VIC20 ||
            Syntax::s.m_currentSystem->m_system==AbstractSystem::PET ||
            Syntax::s.m_currentSystem->m_system==AbstractSystem::NES
            ) {

        Define(new Symbol("screenmemory", "pointer"));
    }
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::AMIGA) {
        Define(new Symbol("copper_bitplane0", "integer"));
        Define(new Symbol("copper_bitplane1", "integer"));
        Define(new Symbol("copper_bitplane2", "integer"));
        Define(new Symbol("copper_bitplane3", "integer"));
        Define(new Symbol("copper_bitplane4", "integer"));
        Define(new Symbol("copper_bitplane0", "integer"));
        Define(new Symbol("copper_mod_even", "integer"));
        Define(new Symbol("copper_mod_odd", "integer"));
        Define(new Symbol("copper_palette", "integer"));
        Define(new Symbol("diwstrt", "integer"));
        Define(new Symbol("diwstop", "integer"));

        Define(new Symbol("ddfstrt", "integer"));
        Define(new Symbol("ddfstop", "integer"));
        Define(new Symbol("copper_resolution", "integer"));
        Define(new Symbol("copper_custom", "integer"));
/*        Define(new Symbol("copper_wait_for_raster", "integer"));
        Define(new Symbol("copper_palette_start", "integer"));
        Define(new Symbol("copper_scroll", "integer"));
*/

    }

}

bool SymbolTable::exists(QString name) {
    if (m_constants.contains(name.toUpper()))
        return true;

    if (m_symbols.contains(name))
        return true;

    return false;
}

QString SymbolTable::findSimilarSymbol(QString sim, float percentage, int n, QStringList procedures)
{
    for (QString s:m_symbols.keys()) {
        if (Util::QStringIsSimilar(sim,s,percentage,n,Qt::CaseInsensitive))
            return s;
    }
    for (QString s:procedures) {
        if (Util::QStringIsSimilar(sim,s,percentage,n,Qt::CaseInsensitive))
            return s;
    }
    for (QString s:m_constants.keys()) {
        if (Util::QStringIsSimilar(sim,s,percentage,n,Qt::CaseInsensitive))
            return s;
    }
    return "";
}

QStringList SymbolTable::getUnusedVariables()
{
    QStringList lst;
    for (QString s : m_symbols.keys()) {
        if (!m_symbols[s]->isUsed)
            lst<<s;
    }
    return lst;
}

Symbol *SymbolTable::Lookup(QString name, int lineNumber, bool isAddress) {
//            name = name.toUpper();
    if (m_constants.contains(name.toUpper())) {
        return m_constants[name.toUpper()];
    }
    // Create address on the fly

    if (name.startsWith("$")) name=name.toUpper();
    //        qDebug() <<name << " exists: " <<m_symbols.contains(name) ;

    // if ((isAddress || name.startsWith("$")) && !m_symbols.contains(name) ) {
    if ((name.startsWith("$")) && !m_symbols.contains(name) ) {
        //            qDebug() << "Creating new symbol:" << name;
        Symbol* s = new Symbol(name, "address");
        s->isUsed = true;
        m_symbols[name] = s;
        return s;
    }

    QString localName = m_currentProcedure+name;
//    qDebug() << "localName: " << localName;
    if (!m_symbols.contains(name) && !m_symbols.contains(localName)) {

        QString similarSymbol = findSimilarSymbol(name,65,2,QStringList());
        QString em = "";
        if (similarSymbol!="") {
            em+="Did you mean '<font color=\"#A080FF\">"+similarSymbol+"</font>'?<br>";
        }
        ErrorHandler::e.Error("Could not find variable '<font color=\"#FF8080\">" + name + "'</font>.<br>"+em, lineNumber);
        return nullptr;
    }

    //qDebug() << name << " " << m_symbols[name]->m_type;
    //        qDebug() << "FOUND "<< name;
//    qDebug() << "SymbolTable Symbols used : " << name <<m_symbols[name]->isUsed;
    // Prioritize local name
  //  qDebug() <<m_symbols.keys();
//    exit(1);
    if (m_symbols.contains(localName)) {
    //    qDebug() << "Found local name " << localName;
        m_symbols[localName]->isUsed = true;
        return m_symbols[localName];

    }
    m_symbols[name]->isUsed = true;
    return m_symbols[name];
}

Symbol *SymbolTable::LookupVariables(QString name, int lineNumber) {
    if (!m_symbols.contains(name)) {
        ErrorHandler::e.Error("Symbol/variable '" + name + "' does not exist in the current scope", lineNumber);
        return nullptr;
    }
    m_symbols[name]->isUsed=true;
    return m_symbols[name];
}

Symbol *SymbolTable::LookupConstants(QString name) {
    if (m_constants.contains(name)) {
        return m_constants[name];
    }
    return nullptr;
}


TokenType::Type Symbol::getTokenType() {
    //qDebug() << "gettokentype: " <<m_name <<" : "<<m_type;
    if (m_type.toLower()=="integer")
        return TokenType::INTEGER;
    if (m_type.toLower()=="word")
        return TokenType::INTEGER;
    if (m_type.toLower()=="float")
        return TokenType::REAL;
    if (m_type.toLower()=="address")
        return TokenType::ADDRESS;
    if (m_type.toLower()=="pointer")
        return TokenType::POINTER;
    if (m_type.toLower()=="byte")
        return TokenType::BYTE;
    if (m_type.toLower()=="string")
        return TokenType::STRING;
    if (m_type.toLower()=="cstring")
        return TokenType::CSTRING;
    if (m_type.toLower()=="incbin")
        return TokenType::INCBIN;
    if (m_type.toLower()=="long")
        return TokenType::LONG;
    if (m_type.toLower()=="incsid")
        return TokenType::INCSID;
    return TokenType::NADA;
}

Symbol::Symbol(QString name, QString type) {
    m_name = name;
    m_type = type;
    m_value = new PVar();
}

Symbol::Symbol(QString name, QString type, float var) {
    m_name = name;
    m_type = type;
    m_value = new PVar(var);

}

Symbol::Symbol(QString name, QString type, QString var) {
    m_name = name;
    m_type = type;
    m_value = new PVar(var);
}
