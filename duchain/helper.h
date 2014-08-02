/*************************************************************************************
 *  Copyright (C) 2013 by Andrea Scarpino <scarpino@kde.org>                         *
 *                                                                                   *
 *  This program is free software; you can redistribute it and/or                    *
 *  modify it under the terms of the GNU General Public License                      *
 *  as published by the Free Software Foundation; either version 2                   *
 *  of the License, or (at your option) any later version.                           *
 *                                                                                   *
 *  This program is distributed in the hope that it will be useful,                  *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
 *  GNU General Public License for more details.                                     *
 *                                                                                   *
 *  You should have received a copy of the GNU General Public License                *
 *  along with this program; if not, write to the Free Software                      *
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA   *
 *************************************************************************************/

#ifndef QMLJSDUCHAINHELPERS_H_
#define QMLJSDUCHAINHELPERS_H_

#include <language/duchain/declaration.h>
#include <libs/qmljs/parser/qmljsast_p.h>

#include "duchainexport.h"

namespace QmlJS
{
using namespace KDevelop;

    /**
     * QML attribute value, with its source location
     */
    struct QMLAttributeValue
    {
        QString value;
        AST::SourceLocation location;
    };

    /**
     * Find the declaration for the specified identifier.
     *
     * If no declaration was found, it will return a null pointer.
     *
     * @param id The qualified identifier that identifies our node.
     * @param context A pointer to the DUContext of this node.
     *
     * @note This method already acquires a read lock for the DUChain.
     */
    KDEVQMLJSDUCHAIN_EXPORT DeclarationPointer getDeclaration(const QualifiedIdentifier& id,
                                                              const DUContext* context,
                                                              bool searchInParent = true);

    /**
     * Find the declaration for the specified identifier, with "onFoo" replaced
     * with "Foo".
     *
     * This function is an extended version of getDeclaration and can resolve
     * QML slots to their corresponding signals.
     */
    KDEVQMLJSDUCHAIN_EXPORT DeclarationPointer getDeclarationOrSignal(const QualifiedIdentifier& id,
                                                                      const DUContext* context,
                                                                      bool searchInParent = true);

    /**
     * Find an attribute of a QML object
     */
    KDEVQMLJSDUCHAIN_EXPORT AST::Statement* getQMLAttribute(AST::UiObjectMemberList* members,
                                                            const QString &attribute);

    /**
     * Get the value of a QML string, identifier or boolean expression
     */
    KDEVQMLJSDUCHAIN_EXPORT QString getNodeValue(AST::Node* node);

    /**
     * Find the string value of a QML attribute
     */
    KDEVQMLJSDUCHAIN_EXPORT QMLAttributeValue getQMLAttributeValue(AST::UiObjectMemberList* members,
                                                                   const QString& attribute);

    /**
     * Get the internal context of a declaration
     */
    KDEVQMLJSDUCHAIN_EXPORT DUContext* getInternalContext(const DeclarationPointer &declaration);

    /**
     * Get the owner of a context
     *
     * If the context is of type Function and has no owner, then its parent context
     * is used to find the owner.
     */
    KDEVQMLJSDUCHAIN_EXPORT Declaration* getOwnerOfContext(const DUContext* context);

    /**
     * Build a zero-length range on the line of the given SourceLocation
     */
    KDEVQMLJSDUCHAIN_EXPORT RangeInRevision emptyRangeOnLine(const AST::SourceLocation& location);

    /**
     * Import the internal context of a declaration in a context.
     *
     * This function is mainly useful because it contains a workaround for a strange
     * behavior of DUContext::Import (that stores the owner of the context being
     * imported and indirectly re-resolves the context, instead of directly
     * storing it)
     *
     * @note The DUChain write lock must be held
     */
    KDEVQMLJSDUCHAIN_EXPORT void importDeclarationInContext(DUContext* context,
                                                            const DeclarationPointer& declaration);

    /**
     * Import the internal context of "Object" (the Javascript base type) in a context.
     *
     * @note The DUChain write lock must be held
     */
    KDEVQMLJSDUCHAIN_EXPORT void importObjectContext(DUContext* context,
                                                     TopDUContext* topContext);

    /**
     * Return whether an identifier is "prototype" or "__proto__" or any future
     * identifier that should be considered to represent a prototype.
     */
    KDEVQMLJSDUCHAIN_EXPORT bool isPrototypeIdentifier(const QString& identifier);


} // End of namespace QmlJS

#endif /* QMLJSDUCHAINHELPERS_H_ */
