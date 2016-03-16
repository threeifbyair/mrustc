/*
 */
#include "ast.hpp"

void AST_InitProvidedModule_Impls();

AST::Module g_compiler_module;
AST::Path   g_copy_marker_path;
AST::Path   g_sized_marker_path;

void AST_InitProvidedModule()
{
    // "struct str([u8])"
    ::std::vector<AST::StructItem>  fields;
    fields.push_back( AST::StructItem(AST::MetaItems(), false, "", TypeRef(TypeRef::TagUnsizedArray(), Span(), TypeRef(Span(), CORETYPE_U8))) );
    g_compiler_module.add_struct(true, "str", AST::Struct(AST::GenericParams(), mv$(fields)), AST::MetaItems());
    
    // TODO: Defer this until AFTER 
    AST_InitProvidedModule_Impls();
}

void AST_InitProvidedModule_Impls()
{
    if( !g_copy_marker_path.is_valid() ) {
        g_copy_marker_path = AST::Path( "", {AST::PathNode("marker"),AST::PathNode("Copy")} );
    }
    
    if( !g_sized_marker_path.is_valid() ) {
        g_sized_marker_path = AST::Path( "", {AST::PathNode("marker"),AST::PathNode("Sized")} );
    }
    
    #define impl(trait, type) \
        g_compiler_module.add_impl(AST::Impl(AST::MetaItems(), AST::GenericParams(), type, trait))
    impl(g_copy_marker_path, TypeRef(Span(), CORETYPE_U8));
    impl(g_copy_marker_path, TypeRef(Span(), CORETYPE_U16));
    impl(g_copy_marker_path, TypeRef(Span(), CORETYPE_U32));
    impl(g_copy_marker_path, TypeRef(Span(), CORETYPE_U64));
    impl(g_copy_marker_path, TypeRef(Span(), CORETYPE_UINT));
    impl(g_copy_marker_path, TypeRef(Span(), CORETYPE_I8));
    impl(g_copy_marker_path, TypeRef(Span(), CORETYPE_I16));
    impl(g_copy_marker_path, TypeRef(Span(), CORETYPE_I32));
    impl(g_copy_marker_path, TypeRef(Span(), CORETYPE_I64));
    impl(g_copy_marker_path, TypeRef(Span(), CORETYPE_INT));
    impl(g_copy_marker_path, TypeRef(Span(), CORETYPE_F32));
    impl(g_copy_marker_path, TypeRef(Span(), CORETYPE_F64));
    
    // A hacky default impl of 'Sized', with a negative impl on [T]
    impl(g_sized_marker_path, TypeRef());
    
    {
        AST::GenericParams tps;
        tps.add_ty_param( AST::TypeParam("T") );
        g_compiler_module.add_neg_impl(AST::ImplDef(
            AST::MetaItems(), ::std::move(tps),
            g_sized_marker_path,
            TypeRef(TypeRef::TagUnsizedArray(), Span(), TypeRef(TypeRef::TagArg(), "T"))
            ));
    }
}

